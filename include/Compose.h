#pragma once
#ifndef COMPOSE_H
#define COMPOSE_H

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include "Decoration.h"
using namespace std;
class Compose
{
public:
    string name;
    int level;
    Compose(string name, int l) : name(name), level(l){}
    string getName() const
    {
        return name;
    }
    virtual ~Compose() {}
    virtual void draw(Icon &icon, Style &style, bool last) const = 0; // last判断是否最后一行
    virtual void add(shared_ptr<Compose> component) {};
    virtual bool getIsLeaf() const = 0;
};

vector<string> outBuffer;// 输出缓冲区

class Leaf : public Compose
{
private:
    string value;
    bool isLeaf;
public:
    
    Leaf(string name,int level, string value="") : Compose(name, level), value(value), isLeaf(true) {}
    void draw(Icon &icon, Style &style, bool last) const override
    {
        // 画图标即可
        string s;
        s += icon.getLeafNode() + getName();
        if(value != ""){
            s += ": " + value;
        }
        s += " ";
        outBuffer.push_back(move(s));
    }
    bool getIsLeaf() const override
    {
        return isLeaf;
    }
};

class Container : public Compose
{
private:
    vector<shared_ptr<Compose>> children;
    bool isLeaf;
public:
    Container() : Compose("", 0), isLeaf(false) {}
    Container(string name,int level) : Compose(name, level), isLeaf(false) {}
    void draw(Icon &icon, Style &style, bool last) const override
    {
        // Style format从前往后分别是：左边框0，右边框1，中间框2，连接线3, 横线4, 上连接5, 左上角6, 右上角7, 左下角8, 右下角9
        // 每个container画自己孩子
        if(level == 0){
            // 根节点
            for(int i=0;i<children.size();++i){
                if(i == 0){
                    string s = style.getFormat(6) + style.getFormat(4) + icon.getMiddleNode();
                    s += children[i]->getName() + " ";
                    outBuffer.push_back(move(s));
                    children[i]->draw(icon, style, false);
                }
                else if(i == children.size() - 1){
                    // 最后一个孩子
                    if(children[i]->getIsLeaf()){
                        // 叶子节点，不需要画连接线
                        string s = style.getFormat(8) + style.getFormat(4);
                        children[i]->draw(icon, style, true);// buffer最后一个是叶子节点信息
                        outBuffer.back() = s + outBuffer.back();
                    }
                    else{
                        string s = style.getFormat(0) + style.getFormat(4) + icon.getMiddleNode();
                        s += children[i]->getName() + " ";
                        outBuffer.push_back(move(s));
                        children[i]->draw(icon, style, true);
                    }
                }
                else{
                    string s = style.getFormat(2) + style.getFormat(4) + icon.getMiddleNode();
                    s += children[i]->getName() + " ";
                    outBuffer.push_back(move(s));
                    children[i]->draw(icon, style, false);
                }
            }
        }
        else{
            // 其他节点
            // Style format从前往后分别是：左边框0，右边框1，中间框2，连接线3, 横线4, 上连接5, 左上角6, 右上角7, 左下角8, 右下角9
            for(int i=0;i<children.size();++i){
                string space = last && i == children.size()-1 ? (style.getFormat(8)+ style.getFormat(11)+style.getFormat(11)) : style.getFormat(3) + "  " ;
                if(last && i != children.size()-1){
                    space = style.getFormat(10) + "  ";
                }
                for(int j=0;j<level-1;++j){
                    space += style.getFormat(10)+ "  ";
                }
                outBuffer.push_back(move(space));
                if(i == children.size() - 1){
                    // 最后一个孩子
                    if(last){
                        outBuffer.back() += style.getFormat(5);
                    }
                    else{
                        outBuffer.back() += style.getFormat(0);
                    }
                    outBuffer.back() += style.getFormat(4);
                    if(children[i]->getIsLeaf()){
                        // 叶子节点，不需要画连接线
                        children[i]->draw(icon, style, false);// buffer最后一个是叶子节点信息
                        string cs = outBuffer.back();
                        outBuffer.pop_back();
                        outBuffer.back() += move(cs);
                    }
                    else{
                        outBuffer.back() += icon.getMiddleNode();
                        outBuffer.back() += children[i]->getName() + " ";
                        children[i]->draw(icon, style, false);
                    }
                }
                else{
                    outBuffer.back() += style.getFormat(2);
                    outBuffer.back() += style.getFormat(4);
                    
                    if(children[i]->getIsLeaf()){
                        children[i]->draw(icon, style, false);
                        string cs = outBuffer.back();
                        outBuffer.pop_back();
                        outBuffer.back() += move(cs);
                    }
                    else{
                        outBuffer.back() += icon.getMiddleNode();
                        outBuffer.back() += children[i]->getName() + " ";
                        children[i]->draw(icon, style, false);
                    }
                }
            }
        }
    }

    bool getIsLeaf() const override
    {
        return isLeaf;
    }

    void add(shared_ptr<Compose> component) override
    {
        children.push_back(component);
    }

};
#endif