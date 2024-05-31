#pragma once
#ifndef FACTORY_H
#define FACTORY_H
#include"Decoration.h"

class ProductFactory
{
public:
    virtual ~ProductFactory() {}
    virtual void introduce() = 0;
    virtual shared_ptr<Decoration> createIcon(string middleNode, string leafNode){ return make_shared<Icon>("","",Type::Icon_);};
    virtual shared_ptr<Decoration> createStyle(string style, vector<string>& format){ return make_shared<Style>("",Type::Style_);};
};

class IconFactory : public ProductFactory
{
public:
    void introduce() override{
        cout<<"IconFactory need middleNode and leafNode"<<endl;
    }
    shared_ptr<Decoration> createIcon(string middleNode, string leafNode){
        return make_shared<Icon>(middleNode, leafNode, Type::Icon_);
    }
};

class StyleFactory : public ProductFactory
{
public:
    void introduce() override{
        cout<<"StyleFactory need style and format"<<endl;
    }
    shared_ptr<Decoration> createStyle(string style, vector<string>& format){
        shared_ptr<Style> sharedStyle =  make_shared<Style>(style, Type::Style_);
        sharedStyle->setFormat(format);
        return sharedStyle;
    }

};

class TopAbstractFactory
{
public:
    virtual ~TopAbstractFactory() {}
    virtual shared_ptr<ProductFactory> createIconFactory() = 0;
    virtual shared_ptr<ProductFactory> createStyleFactory() = 0;
};


class MyFactory : public TopAbstractFactory
{
public:
    shared_ptr<ProductFactory> createIconFactory() override{
        return make_shared<IconFactory>();
    }
    shared_ptr<ProductFactory> createStyleFactory() override{
        return make_shared<StyleFactory>();
    }
};


#endif