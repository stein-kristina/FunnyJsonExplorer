# Funny JSON Explorer

## 运行环境

WSL2.0（Ubuntu22.04）

## 类图

大图位于`readme.assets/test.svg`内

![test](readme.assets/test.svg)

## 设计模式说明

### 工厂方法

其定义是一个创建产品对象的工厂接口，将产品对象的实际创建工作推迟到具体子工厂类当中

在笔者的UML图中，产品`Icon`，`Style`分别由具体子工厂`IconFactory`，`StyleFactory`生产，两子工厂向上提供创建产品的接口

![image-20240531100134964](readme.assets/image-20240531100134964.png)

### 抽象工厂

其定义为用来生产不同产品族的全部产品。

使用抽象工厂模式一般要满足以下条件：
  1) 系统中有多个产品族，每个具体工厂创建同一族但属于不同等级结构的产品。
  2) 系统一次只可能消费其中某一族产品，即同族的产品一起使用。

最顶层的抽象类`TopAbstractFactory`为抽象工厂，`MyFactory`实现其创建两种工厂的接口，即在此模式中`ProductFactory`是产品

<img src="readme.assets/image-20240531100242174.png" alt="image-20240531100242174" style="zoom:50%;" />

### 建造者模式

指将一个复杂对象的构造与它的表示(给对象属性赋值)分离，使同样的构建过程可以创建不同的表示(给对象属性赋值)，这样的设计模式被称为建造者模式。它是将一个复杂的对象分解为多个简单的对象，然后一步一步构建而成。它将变与不变相分离，即产品的组成部分是不变的，但每一部分是可以灵活选择的。

在笔者的UML中，类`JsonExplorer`就是起到导演的作用，通过设定自身内部的具体工厂来达到生产不同产品的目的。

![image-20240531100643813](readme.assets/image-20240531100643813.png)

### 组合模式

有时又叫作部分-整体模式，它是一种将对象组合成树状的层次结构的模式，用来表示“部分-整体”的关系，使用户对单个对象和组合对象具有一致的访问性。

在笔者的UML中，`Compose`，`Leaf`，`Container`就是这样的关系。`Compose`定义了组合对象的所有接口， `Container`是一个整体，`Leaf`属于其中的部分。

![image-20240531101732463](readme.assets/image-20240531101732463.png)

## 运行

include存放头文件，src存放源码，bin存放编译后的文件，input存放要输入的json文件

编译:

```shell
make build
```

测试：

```shell
make run
```

编译且测试：

```shell
make all
```

运行单例：

```shell
./bin/fje -f <json file> -s <style> -i <icon family>
```

style支持两种：tree和rectangle

icon family支持两种：default和poker

## 可扩展性

要想自定义您自己的工厂，只需继承TopAbstractFactory并实现其接口，通过接口创建您自己的图标工厂和风格工厂即可

## 测试截图

2种风格与2种图标混搭

![image-20240531102740769](readme.assets/image-20240531102740769.png)

![image-20240531102751113](readme.assets/image-20240531102751113.png)
