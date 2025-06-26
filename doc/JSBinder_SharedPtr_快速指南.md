# JSBinder Shared_ptr 快速指南

## 快速开始

### 1. 定义C++类

```cpp
#include <jsbind/JSBind.h>
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    MyClass() { LOGI("MyClass created"); }
    ~MyClass() { LOGI("MyClass destroyed"); }
    
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

### 2. JavaScript使用

```javascript
// 创建对象
let obj = new MyClass();

// 调用方法
obj.doSomething();

// 获取shared_ptr引用
let shared = obj.getShared();

// 多个引用共享同一个对象
let obj2 = shared;
console.log(obj === obj2); // true

// 自动内存管理
obj = null;
obj2 = null;
shared = null;
// C++对象自动销毁
```

## 核心概念

### Traits系统

- `raw_ptr_traits`: 原始指针管理
- `shared_ptr_traits`: 智能指针管理

### 关键差异

| 特性 | raw_ptr_traits | shared_ptr_traits |
|------|----------------|-------------------|
| 内存管理 | 手动delete | 自动管理 |
| 引用计数 | 无 | 自动计数 |
| 生命周期 | 需要手动控制 | 自动控制 |
| 循环引用 | 无问题 | 需要weak_ptr |

## 常用模式

### 1. 参数传递

```cpp
class Example {
public:
    // 接收shared_ptr参数
    void process(std::shared_ptr<MyClass> obj) {
        if (obj) obj->doSomething();
    }
    
    // 返回shared_ptr
    std::shared_ptr<MyClass> create() {
        return std::make_shared<MyClass>();
    }
    
    // 接收const引用（推荐）
    void processConst(const std::shared_ptr<MyClass>& obj) {
        if (obj) obj->doSomething();
    }
};
```

### 2. 容器支持

```cpp
class Container {
public:
    std::vector<std::shared_ptr<MyClass>> getObjects() {
        std::vector<std::shared_ptr<MyClass>> vec;
        vec.push_back(std::make_shared<MyClass>());
        vec.push_back(std::make_shared<MyClass>());
        return vec;
    }
    
    void processObjects(const std::vector<std::shared_ptr<MyClass>>& objs) {
        for (const auto& obj : objs) {
            obj->doSomething();
        }
    }
};
```

### 3. 循环引用处理

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
    void setParent(std::weak_ptr<Parent> parent) { // 使用weak_ptr
        parent_ = parent;
    }
    
private:
    std::weak_ptr<Parent> parent_;
};
```

## 最佳实践

### ✅ 推荐做法

1. **使用shared_ptr_traits**
   ```cpp
   jsbind::class_<MyClass, jsbind::shared_ptr_traits> binding;
   ```

2. **继承enable_shared_from_this**
   ```cpp
   class MyClass : public std::enable_shared_from_this<MyClass>
   ```

3. **使用const引用**
   ```cpp
   void process(const std::shared_ptr<MyClass>& obj);
   ```

4. **避免循环引用**
   ```cpp
   std::weak_ptr<Parent> parent_; // 而不是 std::shared_ptr
   ```

### ❌ 避免做法

1. **混用traits类型**
   ```cpp
   // 不要这样做
   jsbind::class_<MyClass, jsbind::raw_ptr_traits> binding;
   ```

2. **返回raw pointer**
   ```cpp
   // 不要这样做
   MyClass* getRaw() { return new MyClass(); }
   ```

3. **忽略循环引用**
   ```cpp
   // 不要这样做
   std::shared_ptr<Parent> parent_; // 可能导致内存泄漏
   ```

## 常见问题

### Q: 如何检查对象是否为null？

```cpp
void process(std::shared_ptr<MyClass> obj) {
    if (obj) {
        // 对象存在
        obj->doSomething();
    } else {
        // 对象为null
        LOGI("Object is null");
    }
}
```

### Q: 如何获取引用计数？

```cpp
int getUseCount() const {
    return shared_from_this().use_count();
}
```

### Q: 如何处理循环引用？

```cpp
// 使用weak_ptr
std::weak_ptr<Parent> parent_;

// 使用时检查
if (auto parent = parent_.lock()) {
    parent->doSomething();
}
```

## 性能考虑

1. **避免不必要的拷贝**
   ```cpp
   // 好：使用const引用
   void process(const std::shared_ptr<MyClass>& obj);
   
   // 差：值传递
   void process(std::shared_ptr<MyClass> obj);
   ```

2. **使用移动语义**
   ```cpp
   std::shared_ptr<MyClass> create() {
       return std::make_shared<MyClass>();
   }
   ```

3. **批量操作优化**
   ```cpp
   void processBatch(const std::vector<std::shared_ptr<MyClass>>& batch) {
       for (const auto& item : batch) {
           item->doSomething();
       }
   }
   ```

## 总结

JSBinder的shared_ptr支持提供了：

- ✅ 自动内存管理
- ✅ 类型安全
- ✅ 容器支持
- ✅ 循环引用处理
- ✅ 性能优化

关键要点：
1. 使用`shared_ptr_traits`
2. 继承`enable_shared_from_this`
3. 避免循环引用
4. 合理使用const引用
5. 进行充分测试 