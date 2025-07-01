# JSBinder模块Shared_ptr支持实现文档

## 概述

JSBinder模块新增了对C++ `std::shared_ptr`的完整支持，允许JavaScript代码安全地管理C++对象的生命周期。该实现基于traits模式设计，提供了统一的接口来处理不同类型的指针类型。

## 架构设计

### 核心组件

#### 1. Traits系统

JSBinder使用traits模式来抽象不同类型的指针管理：

```cpp
// 原始指针traits
struct raw_ptr_traits {
    using pointer_type = void*;
    using object_pointer_type = T*;
    using object_id = void*;
    
    static object_id pointer_id(void* ptr) { return ptr; }
    static pointer_type to_pointer_type(object_id id) { return id; }
    
    template<typename T, typename... Args>
    static object_pointer_type<T> create(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }
    
    template<typename T>
    static void destroy(object_pointer_type<T> const& ptr) {
        delete ptr;
    }
};

// shared_ptr traits
struct shared_ptr_traits {
    using pointer_type = std::shared_ptr<void>;
    using object_pointer_type = std::shared_ptr<T>;
    using object_id = void*;
    
    static object_id pointer_id(pointer_type const& ptr) { return ptr.get(); }
    static pointer_type to_pointer_type(object_id id) { 
        return std::shared_ptr<void>(id, [](void*) {}); 
    }
    
    template<typename T, typename... Args>
    static object_pointer_type<T> create(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    static void destroy(object_pointer_type<T> const&) {
        // shared_ptr自动管理生命周期，无需手动销毁
    }
};
```

#### 2. 类型检测系统

```cpp
template<typename T>
struct is_shared_ptr : std::false_type {};

template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
struct is_wrapped_class : std::conjunction<
    std::is_class<T>, 
    std::negation<internal::is_value_object<T>>,
    std::negation<internal::is_mapping<T>>,
    std::negation<internal::is_sequence<T>>,
    std::negation<internal::is_shared_ptr<T>>
> {};
```

#### 3. 值转换系统

```cpp
template <typename T> 
class ValueTraits<std::shared_ptr<T>, std::enable_if_t<internal::is_wrapped_class<T>::value>> {
public:
    static jsvm_value ToJs(std::shared_ptr<T> value, bool callDestructor = true) {
        if (value == nullptr) {
            return makeNull();
        }
        return wrapCppObject<T, shared_ptr_traits>(value, callDestructor);
    }
    
    static std::shared_ptr<T> ToCpp(jsvm_value value) {
        DEBUG_CHECK(value != nullptr);
        return unwrapCppObject<T, shared_ptr_traits>(value);
    }
    
    static bool is(jsvm_value value) {
        return isWrappedClassOf<T>();
    }
};
```

### 生命周期管理

#### 1. 对象包装

```cpp
template <typename ClassType, typename Traits>
jsvm_value ClassRegistry<ClassType, Traits>::wrapCppObject(object_id objectPointer, bool callDestructor) {
    GET_ENV
    jsvm_status status;
    jsvm_value result;
    
    status = jsvm_wrap(env, result, objectPointer, 
                      static_cast<jsvm_finalize>(internal::destructor<ClassType, Traits>), 
                      nullptr, &objectRef);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    
    objects_.emplace(Traits::to_pointer_type(objectPointer), 
                    std::make_unique<ObjectRegistry>(ObjectRegistry{objectRef, callDestructor}));
    return result;
}
```

#### 2. 对象解包

```cpp
template <typename ClassType, typename Traits>
typename Traits::template object_pointer_type<ClassType> 
ClassRegistry<ClassType, Traits>::unwrapCppObject(jsvm_value value) {
    GET_ENV
    jsvm_status status;
    void* nativeObject;
    
    status = jsvm_unwrap(env, value, &nativeObject);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    
    return Traits::to_pointer_type(nativeObject);
}
```

#### 3. 析构函数处理

```cpp
template <typename ClassType, typename Traits> 
static void destructor(jsvm_env env, void *nativeObject, void * /*finalize_hint*/) {
    ClassRegistry<ClassType, Traits> *classRegistry = 
        static_cast<ClassRegistry<ClassType, Traits>*>(
            ClassRegistryManager::getClassRegistry(type_id<ClassType>()));
    ClassType *object = static_cast<ClassType *>(nativeObject);
    DEBUG_CHECK(object != nullptr);
    classRegistry->removeObject(env, object);
}
```

## 使用指南

### 1. 基本用法

#### 定义支持shared_ptr的C++类

```cpp
#include <jsbind/JSBind.h>
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    MyClass() { LOGI("MyClass::MyClass"); }
    ~MyClass() { LOGI("MyClass::~MyClass"); }
    
    void doSomething() { LOGI("MyClass::doSomething"); }
    std::shared_ptr<MyClass> getShared() { return shared_from_this(); }
    
    static void exportJS(jsbind::Object& context);
};

void MyClass::exportJS(jsbind::Object& context) {
    jsbind::class_<MyClass, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("doSomething", &MyClass::doSomething);
    class_binding.function("getShared", &MyClass::getShared);
    class_binding.constructor<>();
    context.class_("MyClass", class_binding);
}
```

#### JavaScript端使用

```javascript
// 创建shared_ptr对象
let obj = new MyClass();

// 调用方法
obj.doSomething();

// 获取shared_ptr引用
let shared = obj.getShared();

// 多个引用共享同一个对象
let obj2 = shared;
console.log(obj === obj2); // true

// 当所有JavaScript引用都被释放时，C++对象自动销毁
obj = null;
obj2 = null;
shared = null;
```

### 2. 参数传递

#### C++端定义

```cpp
class SharedPtrExample {
public:
    void testRawPointer(MyClass* ptr) {
        LOGI("Received raw pointer: %p", ptr);
    }
    
    void testSharedPointer(std::shared_ptr<MyClass> ptr) {
        LOGI("Received shared_ptr: %p, use_count: %ld", 
             ptr.get(), ptr.use_count());
    }
    
    std::shared_ptr<MyClass> createShared() {
        return std::make_shared<MyClass>();
    }
    
    static void exportJS(jsbind::Object& context);
};

void SharedPtrExample::exportJS(jsbind::Object& context) {
    jsbind::class_<SharedPtrExample, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("testRawPointer", &SharedPtrExample::testRawPointer);
    class_binding.function("testSharedPointer", &SharedPtrExample::testSharedPointer);
    class_binding.function("createShared", &SharedPtrExample::createShared);
    class_binding.constructor<>();
    context.class_("SharedPtrExample", class_binding);
}
```

#### JavaScript端调用

```javascript
let example = new SharedPtrExample();

// 创建对象
let obj = new MyClass();

// 传递raw pointer
example.testRawPointer(obj);

// 传递shared_ptr
example.testSharedPointer(obj);

// 获取shared_ptr
let shared = example.createShared();
```

### 3. 容器支持

#### C++端定义

```cpp
class ContainerExample {
public:
    std::vector<std::shared_ptr<MyClass>> getSharedVector() {
        std::vector<std::shared_ptr<MyClass>> vec;
        vec.push_back(std::make_shared<MyClass>());
        vec.push_back(std::make_shared<MyClass>());
        return vec;
    }
    
    void processSharedVector(const std::vector<std::shared_ptr<MyClass>>& vec) {
        LOGI("Processing vector with %zu elements", vec.size());
        for (const auto& item : vec) {
            item->doSomething();
        }
    }
    
    static void exportJS(jsbind::Object& context);
};

void ContainerExample::exportJS(jsbind::Object& context) {
    jsbind::class_<ContainerExample, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("getSharedVector", &ContainerExample::getSharedVector);
    class_binding.function("processSharedVector", &ContainerExample::processSharedVector);
    class_binding.constructor<>();
    context.class_("ContainerExample", class_binding);
}
```

#### JavaScript端使用

```javascript
let container = new ContainerExample();

// 获取shared_ptr向量
let vec = container.getSharedVector();
console.log(vec.length); // 2

// 处理shared_ptr向量
container.processSharedVector(vec);

// 向量中的对象也是shared_ptr
vec[0].doSomething();
```

## 最佳实践

### 1. 内存管理

#### 推荐做法

```cpp
// ✅ 使用shared_ptr_traits确保自动内存管理
class GoodExample : public std::enable_shared_from_this<GoodExample> {
public:
    GoodExample() = default;
    ~GoodExample() = default;
    
    // 返回shared_ptr而不是raw pointer
    std::shared_ptr<GoodExample> getShared() { 
        return shared_from_this(); 
    }
    
    static void exportJS(jsbind::Object& context) {
        jsbind::class_<GoodExample, jsbind::shared_ptr_traits> class_binding;
        class_binding.function("getShared", &GoodExample::getShared);
        class_binding.constructor<>();
        context.class_("GoodExample", class_binding);
    }
};
```

#### 避免的做法

```cpp
// ❌ 不要混用raw_ptr_traits和shared_ptr
class BadExample {
public:
    // 可能导致内存泄漏
    GoodExample* getRawPointer() { 
        return new GoodExample(); 
    }
    
    static void exportJS(jsbind::Object& context) {
        // 混用traits可能导致问题
        jsbind::class_<BadExample, jsbind::raw_ptr_traits> class_binding;
        class_binding.function("getRawPointer", &BadExample::getRawPointer);
        class_binding.constructor<>();
        context.class_("BadExample", class_binding);
    }
};
```

### 2. 循环引用处理

```cpp
class Parent : public std::enable_shared_from_this<Parent> {
public:
    void setChild(std::shared_ptr<Child> child) {
        child_ = child;
        child->setParent(shared_from_this());
    }
    
private:
    std::shared_ptr<Child> child_;
};

class Child : public std::enable_shared_from_this<Child> {
public:
    void setParent(std::weak_ptr<Parent> parent) { // 使用weak_ptr避免循环引用
        parent_ = parent;
    }
    
private:
    std::weak_ptr<Parent> parent_;
};
```

### 3. 性能优化

```cpp
class OptimizedExample : public std::enable_shared_from_this<OptimizedExample> {
public:
    // 使用const引用避免不必要的拷贝
    void processData(const std::shared_ptr<MyClass>& data) {
        data->doSomething();
    }
    
    // 使用移动语义
    std::shared_ptr<MyClass> createOptimized() {
        return std::make_shared<MyClass>();
    }
    
    // 批量操作时使用引用
    void processBatch(const std::vector<std::shared_ptr<MyClass>>& batch) {
        for (const auto& item : batch) {
            item->doSomething();
        }
    }
};
```

## 测试用例

### 1. 基本功能测试

```cpp
void testBasicSharedPtr() {
    // 创建shared_ptr对象
    auto obj = std::make_shared<MyClass>();
    assert(obj.use_count() == 1);
    
    // 传递到JavaScript
    jsvm_value jsObj = jsbind::Make(obj);
    
    // 从JavaScript获取
    auto retrieved = jsbind::as<std::shared_ptr<MyClass>>(jsObj);
    assert(retrieved == obj);
    assert(obj.use_count() == 2);
    
    // 清理
    jsObj = nullptr;
    assert(obj.use_count() == 1);
}
```

### 2. 生命周期测试

```cpp
void testLifecycle() {
    int destructorCount = 0;
    
    {
        auto obj = std::make_shared<TestClass>(destructorCount);
        assert(destructorCount == 0);
        
        jsvm_value jsObj = jsbind::Make(obj);
        assert(obj.use_count() == 2);
        
        // 释放C++引用
        obj.reset();
        assert(destructorCount == 0); // JavaScript仍持有引用
        
        // 释放JavaScript引用
        jsObj = nullptr;
        assert(destructorCount == 1); // 现在应该被销毁
    }
}
```

### 3. 容器测试

```cpp
void testContainer() {
    std::vector<std::shared_ptr<MyClass>> vec;
    vec.push_back(std::make_shared<MyClass>());
    vec.push_back(std::make_shared<MyClass>());
    
    jsvm_value jsVec = jsbind::Make(vec);
    auto retrieved = jsbind::as<std::vector<std::shared_ptr<MyClass>>>(jsVec);
    
    assert(retrieved.size() == 2);
    assert(retrieved[0] == vec[0]);
    assert(retrieved[1] == vec[1]);
}
```

## 错误处理

### 1. 空指针检查

```cpp
template <typename T> 
class ValueTraits<std::shared_ptr<T>> {
public:
    static jsvm_value ToJs(std::shared_ptr<T> value, bool callDestructor = true) {
        if (!value) {
            return makeNull();
        }
        return wrapCppObject<T, shared_ptr_traits>(value, callDestructor);
    }
    
    static std::shared_ptr<T> ToCpp(jsvm_value value) {
        if (isNull(value)) {
            return nullptr;
        }
        return unwrapCppObject<T, shared_ptr_traits>(value);
    }
};
```

### 2. 类型安全检查

```cpp
template <typename T>
std::shared_ptr<T> safeUnwrap(jsvm_value value) {
    if (!jsbind::isWrappedClassOf<T>()) {
        throw std::runtime_error("Invalid object type");
    }
    return jsbind::as<std::shared_ptr<T>>(value);
}
```

## 总结

JSBinder模块的shared_ptr支持提供了完整的C++智能指针与JavaScript的互操作能力。通过traits模式的设计，系统能够统一处理不同类型的指针，同时保证内存安全和生命周期管理的正确性。

主要特性：
- 自动内存管理
- 类型安全的转换
- 容器支持
- 循环引用处理
- 性能优化

使用建议：
1. 优先使用shared_ptr_traits进行对象绑定
2. 避免混用不同的traits类型
3. 注意循环引用的处理
4. 合理使用const引用和移动语义
5. 进行充分的测试验证 