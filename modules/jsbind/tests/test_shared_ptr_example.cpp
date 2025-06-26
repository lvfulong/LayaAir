#include <jsbind/JSBind.h>
#include <jsvm/JSEnv.h>
#include <utils/Log.h>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

// 测试用的基础类
class TestObject : public std::enable_shared_from_this<TestObject> {
public:
    TestObject(const std::string& name = "default") : name_(name) {
        LOGI("TestObject::TestObject(%s)", name_.c_str());
    }
    
    ~TestObject() {
        LOGI("TestObject::~TestObject(%s)", name_.c_str());
    }
    
    void doSomething() {
        LOGI("TestObject::doSomething(%s)", name_.c_str());
    }
    
    std::string getName() const {
        return name_;
    }
    
    void setName(const std::string& name) {
        name_ = name;
    }
    
    std::shared_ptr<TestObject> getShared() {
        return shared_from_this();
    }
    
    int getUseCount() const {
        return shared_from_this().use_count();
    }
    
    static void exportJS(jsbind::Object& context);
    
private:
    std::string name_;
};

// 容器测试类
class ContainerTest {
public:
    ContainerTest() {
        LOGI("ContainerTest::ContainerTest");
    }
    
    ~ContainerTest() {
        LOGI("ContainerTest::~ContainerTest");
    }
    
    std::vector<std::shared_ptr<TestObject>> createVector() {
        std::vector<std::shared_ptr<TestObject>> vec;
        vec.push_back(std::make_shared<TestObject>("obj1"));
        vec.push_back(std::make_shared<TestObject>("obj2"));
        vec.push_back(std::make_shared<TestObject>("obj3"));
        LOGI("ContainerTest::createVector - created %zu objects", vec.size());
        return vec;
    }
    
    void processVector(const std::vector<std::shared_ptr<TestObject>>& vec) {
        LOGI("ContainerTest::processVector - processing %zu objects", vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            LOGI("  Object %zu: %s (use_count: %d)", i, vec[i]->getName().c_str(), vec[i]->getUseCount());
            vec[i]->doSomething();
        }
    }
    
    std::shared_ptr<TestObject> getFirstObject(const std::vector<std::shared_ptr<TestObject>>& vec) {
        if (!vec.empty()) {
            LOGI("ContainerTest::getFirstObject - returning %s", vec[0]->getName().c_str());
            return vec[0];
        }
        return nullptr;
    }
    
    static void exportJS(jsbind::Object& context);
};

// 参数传递测试类
class ParameterTest {
public:
    ParameterTest() {
        LOGI("ParameterTest::ParameterTest");
    }
    
    ~ParameterTest() {
        LOGI("ParameterTest::~ParameterTest");
    }
    
    void testRawPointer(TestObject* obj) {
        if (obj) {
            LOGI("ParameterTest::testRawPointer - received %s", obj->getName().c_str());
        } else {
            LOGI("ParameterTest::testRawPointer - received null pointer");
        }
    }
    
    void testSharedPointer(std::shared_ptr<TestObject> obj) {
        if (obj) {
            LOGI("ParameterTest::testSharedPointer - received %s (use_count: %d)", 
                 obj->getName().c_str(), obj->use_count());
        } else {
            LOGI("ParameterTest::testSharedPointer - received null shared_ptr");
        }
    }
    
    void testConstSharedPointer(const std::shared_ptr<TestObject>& obj) {
        if (obj) {
            LOGI("ParameterTest::testConstSharedPointer - received %s (use_count: %d)", 
                 obj->getName().c_str(), obj->use_count());
        } else {
            LOGI("ParameterTest::testConstSharedPointer - received null shared_ptr");
        }
    }
    
    std::shared_ptr<TestObject> createAndReturn() {
        auto obj = std::make_shared<TestObject>("created_by_parameter_test");
        LOGI("ParameterTest::createAndReturn - created %s", obj->getName().c_str());
        return obj;
    }
    
    static void exportJS(jsbind::Object& context);
};

// 循环引用测试类
class Parent : public std::enable_shared_from_this<Parent> {
public:
    Parent(const std::string& name = "parent") : name_(name) {
        LOGI("Parent::Parent(%s)", name_.c_str());
    }
    
    ~Parent() {
        LOGI("Parent::~Parent(%s)", name_.c_str());
    }
    
    void setChild(std::shared_ptr<Parent> child) {
        child_ = child;
        LOGI("Parent::setChild(%s) - child: %s", name_.c_str(), 
             child ? child->getName().c_str() : "null");
    }
    
    std::shared_ptr<Parent> getChild() const {
        return child_;
    }
    
    std::string getName() const {
        return name_;
    }
    
    int getUseCount() const {
        return shared_from_this().use_count();
    }
    
    static void exportJS(jsbind::Object& context);
    
private:
    std::string name_;
    std::shared_ptr<Parent> child_;
};

// 导出函数实现
void TestObject::exportJS(jsbind::Object& context) {
    jsbind::class_<TestObject, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("doSomething", &TestObject::doSomething);
    class_binding.function("getName", &TestObject::getName);
    class_binding.function("setName", &TestObject::setName);
    class_binding.function("getShared", &TestObject::getShared);
    class_binding.function("getUseCount", &TestObject::getUseCount);
    class_binding.constructor<>();
    class_binding.constructor<const std::string&>();
    context.class_("TestObject", class_binding);
}

void ContainerTest::exportJS(jsbind::Object& context) {
    jsbind::class_<ContainerTest, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("createVector", &ContainerTest::createVector);
    class_binding.function("processVector", &ContainerTest::processVector);
    class_binding.function("getFirstObject", &ContainerTest::getFirstObject);
    class_binding.constructor<>();
    context.class_("ContainerTest", class_binding);
}

void ParameterTest::exportJS(jsbind::Object& context) {
    jsbind::class_<ParameterTest, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("testRawPointer", &ParameterTest::testRawPointer);
    class_binding.function("testSharedPointer", &ParameterTest::testSharedPointer);
    class_binding.function("testConstSharedPointer", &ParameterTest::testConstSharedPointer);
    class_binding.function("createAndReturn", &ParameterTest::createAndReturn);
    class_binding.constructor<>();
    context.class_("ParameterTest", class_binding);
}

void Parent::exportJS(jsbind::Object& context) {
    jsbind::class_<Parent, jsbind::shared_ptr_traits> class_binding;
    class_binding.function("setChild", &Parent::setChild);
    class_binding.function("getChild", &Parent::getChild);
    class_binding.function("getName", &Parent::getName);
    class_binding.function("getUseCount", &Parent::getUseCount);
    class_binding.constructor<>();
    class_binding.constructor<const std::string&>();
    context.class_("Parent", class_binding);
}

// 测试函数
void runSharedPtrTests() {
    LOGI("=== 开始Shared_ptr测试 ===");
    
    // 基本功能测试
    {
        LOGI("--- 基本功能测试 ---");
        auto obj = std::make_shared<TestObject>("test_obj");
        assert(obj.use_count() == 1);
        
        // 测试shared_from_this
        auto shared = obj->getShared();
        assert(shared == obj);
        assert(obj.use_count() == 2);
        
        LOGI("基本功能测试通过");
    }
    
    // 生命周期测试
    {
        LOGI("--- 生命周期测试 ---");
        int destructorCount = 0;
        
        {
            auto obj = std::make_shared<TestObject>("lifecycle_test");
            assert(obj.use_count() == 1);
            
            // 模拟JavaScript持有引用
            auto jsRef = obj;
            assert(obj.use_count() == 2);
            
            // 释放C++引用
            obj.reset();
            assert(jsRef.use_count() == 1);
            
            // 释放JavaScript引用
            jsRef.reset();
            // 此时对象应该被销毁
        }
        
        LOGI("生命周期测试通过");
    }
    
    // 容器测试
    {
        LOGI("--- 容器测试 ---");
        ContainerTest container;
        
        auto vec = container.createVector();
        assert(vec.size() == 3);
        
        container.processVector(vec);
        
        auto first = container.getFirstObject(vec);
        assert(first == vec[0]);
        assert(first->getName() == "obj1");
        
        LOGI("容器测试通过");
    }
    
    // 参数传递测试
    {
        LOGI("--- 参数传递测试 ---");
        ParameterTest paramTest;
        
        auto obj = std::make_shared<TestObject>("param_test");
        
        paramTest.testRawPointer(obj.get());
        paramTest.testSharedPointer(obj);
        paramTest.testConstSharedPointer(obj);
        
        auto returned = paramTest.createAndReturn();
        assert(returned->getName() == "created_by_parameter_test");
        
        LOGI("参数传递测试通过");
    }
    
    // 循环引用测试
    {
        LOGI("--- 循环引用测试 ---");
        auto parent1 = std::make_shared<Parent>("parent1");
        auto parent2 = std::make_shared<Parent>("parent2");
        
        parent1->setChild(parent2);
        parent2->setChild(parent1);
        
        assert(parent1->getUseCount() >= 2); // 至少有两个引用
        assert(parent2->getUseCount() >= 2);
        
        LOGI("循环引用测试通过");
    }
    
    LOGI("=== Shared_ptr测试完成 ===");
}

// JavaScript测试脚本
const char* testScript = R"(
console.log("=== JavaScript Shared_ptr测试 ===");

// 基本功能测试
console.log("--- 基本功能测试 ---");
let obj = new TestObject("js_test");
obj.doSomething();
console.log("对象名称:", obj.getName());

let shared = obj.getShared();
console.log("共享引用相等:", obj === shared);
console.log("引用计数:", obj.getUseCount());

// 参数传递测试
console.log("--- 参数传递测试 ---");
let paramTest = new ParameterTest();

// 测试raw pointer
paramTest.testRawPointer(obj);

// 测试shared_ptr
paramTest.testSharedPointer(obj);

// 测试const shared_ptr
paramTest.testConstSharedPointer(obj);

// 测试返回值
let returned = paramTest.createAndReturn();
console.log("返回的对象:", returned.getName());

// 容器测试
console.log("--- 容器测试 ---");
let container = new ContainerTest();
let vec = container.createVector();
console.log("向量大小:", vec.length);

container.processVector(vec);

let first = container.getFirstObject(vec);
console.log("第一个对象:", first.getName());

// 循环引用测试
console.log("--- 循环引用测试 ---");
let parent1 = new Parent("js_parent1");
let parent2 = new Parent("js_parent2");

parent1.setChild(parent2);
parent2.setChild(parent1);

console.log("parent1引用计数:", parent1.getUseCount());
console.log("parent2引用计数:", parent2.getUseCount());

// 清理测试
console.log("--- 清理测试 ---");
obj = null;
shared = null;
paramTest = null;
container = null;
vec = null;
first = null;
returned = null;
parent1 = null;
parent2 = null;

console.log("=== JavaScript测试完成 ===");
)";

// 主测试函数
void testSharedPtrExample() {
    LOGI("开始Shared_ptr示例测试");
    
    // 运行C++测试
    runSharedPtrTests();
    
    // 运行JavaScript测试
    try {
        jsvm_env env = jsvm::getEnv();
        
        // 创建导出对象
        jsvm_value exports;
        jsvm_status status = jsvm_create_object(env, &exports);
        assert(status == jsvm_status::jsvm_ok);
        
        jsbind::Object context(exports);
        
        // 导出类
        TestObject::exportJS(context);
        ContainerTest::exportJS(context);
        ParameterTest::exportJS(context);
        Parent::exportJS(context);
        
        // 设置全局对象
        jsvm_value global;
        status = jsvm_get_global(env, &global);
        assert(status == jsvm_status::jsvm_ok);
        
        status = jsvm_set_named_property(env, global, "exports", exports);
        assert(status == jsvm_status::jsvm_ok);
        
        // 执行测试脚本
        jsvm_value result;
        status = jsvm_run_script(env, testScript, "shared_ptr_test.js", &result);
        assert(status == jsvm_status::jsvm_ok);
        
        LOGI("JavaScript测试执行成功");
        
    } catch (const std::exception& e) {
        LOGE("JavaScript测试失败: %s", e.what());
    }
    
    LOGI("Shared_ptr示例测试完成");
} 