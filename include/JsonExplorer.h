#ifndef JSONEXPLORER
#define JSONEXPLORER
#include "Factory.h"
#include"Compose.h"
#include "iterator.h"


class JsonExplorer
{
private:
    // vector<shared_ptr<Compose>> root;
    shared_ptr<Iterator> iterator;
    shared_ptr<TopAbstractFactory> factory;
    shared_ptr<ProductFactory> decorationFactory;

public:
    JsonExplorer()
    {
        iterator = nullptr;
        factory = nullptr;
        decorationFactory = nullptr;
    }

    // 读取文件并加载
    void _load(string filename)
    {
        std::ifstream ifs(filename);
        if(!ifs.is_open()){
            std::printf("file not exist.\n");
            return;
        }
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string raw_json = buffer.str();
        // 格式化json
        // std::string processed_json = preprocess_json(raw_json);
        // 解析json
        json j = json::parse(raw_json);
        iterator = CreateIterator(&j);
    }

    // 设置客户自定义工厂
    void setFactory(shared_ptr<TopAbstractFactory> factory)
    {
        this->factory = move(factory);
    }

    // 建立图标和风格工厂
    shared_ptr<TopAbstractFactory> getProductFactory()
    {
        return factory;
    }

    // 设置装饰工厂
    void setDecorationFactory(shared_ptr<ProductFactory> decorationFactory)
    {
        this->decorationFactory = decorationFactory;
    }

    // 生产产品
    shared_ptr<ProductFactory> getDecorationFactory()
    {
        return decorationFactory;
    }

    // 获取迭代器
    shared_ptr<Iterator> getIterator()
    {
        return iterator;
    }

    // 以矩阵风格画图
    // void drawWithRectangleStyle(Icon &icon){
    //     if(factory == nullptr){
    //         factory.reset(new MyFactory());
    //     }
    //     shared_ptr<ProductFactory> styleFactory = factory->createStyleFactory();
    //     vector<string> rectangleFormat = {"├", "┤", "├", "|", "─", "┴", "┌", "┐", "└", "┘", "|", "─"};
    //     shared_ptr<Decoration> myStyle = styleFactory->createStyle("rectangle", rectangleFormat);
    //     Style* changeStyle = dynamic_cast<Style*>(myStyle.get());
    //     draw(icon, *changeStyle);
    // }


    // // 以树状风格画图
    // void drawWithTreeStyle(Icon &icon){
    //     if(factory == nullptr){
    //         factory.reset(new MyFactory());
    //     }
    //     shared_ptr<ProductFactory> styleFactory = factory->createStyleFactory();
    //     vector<string> treeFormat = {"└", " ", "├", "|", "─", "└", "├", " ", " ", " ", " ", " "};
    //     shared_ptr<Decoration> myStyle = styleFactory->createStyle("tree", treeFormat);
    //     Style* changeStyle = dynamic_cast<Style*>(myStyle.get());
    //     draw(icon, *changeStyle);
    // }


};
#endif