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
    Compose(string name, int l) : name(name), level(l) {}
    string getName() const
    {
        return name;
    }
    virtual ~Compose() {}
    virtual void draw(Icon &icon, Style &style, bool first, bool last, bool globalLast) const = 0; // last判断是否最后一行
    virtual bool getIsLeaf() const = 0;
};

vector<string> outBuffer; // 输出缓冲区

class Leaf : public Compose
{
private:
    string value;
    bool isLeaf;

public:
    Leaf(string name, int level, string value = "") : Compose(name, level), value(value), isLeaf(true) {}
    void draw(Icon &icon, Style &style, bool first, bool last, bool globalLast) const override
    {
        if(first){
            // 全局第一个
            string space;
            space += style.getFormat(6) + style.getFormat(4);
            space += icon.getLeafNode() + getName();
            if(value != "") space += ": " + value;
            else space += " ";
            outBuffer.push_back(move(space));
        }
        else if(last){
            // 内部的最后一个
            string space;
            if(globalLast){
                space += style.getFormat(10) + "  ";
            }
            else space += style.getFormat(3) + "  ";
            for (int j = 0; j < level - 2; ++j)
            {
                space += style.getFormat(10) + "  ";
            }

            if (globalLast)
            {
                space += style.getFormat(2) + style.getFormat(4);
            }
            else
            {
                space += style.getFormat(0) + style.getFormat(4);
            }
            space += icon.getLeafNode() + getName();

            if(value != "") space += ": " + value;
            else space += " ";
            outBuffer.push_back(move(space));
        }
        else if(globalLast){
            string space;
            space += style.getFormat(8) + style.getFormat(11) + style.getFormat(11);
            // 上下级连接
            for(int j = 0; j < level - 2; ++j){
                space += style.getFormat(11) + style.getFormat(11) + style.getFormat(11);
            }
            if(level == 1){
                space = "└" + style.getFormat(4);
            }
            else space += style.getFormat(5) + style.getFormat(4);
            space += icon.getLeafNode() + getName();

            if(value != "") space += ": " + value;
            else space += " ";
            outBuffer.push_back(move(space));
        }
        else{
            string space;
            space += style.getFormat(3) + "  ";
            // level=1的叶子不需要前缀
            if(level == 1) space = "";
            for (int j = 0; j < level - 2; ++j)
            {
                space += style.getFormat(10) +  "  ";
            }
            space += style.getFormat(2) + style.getFormat(4);
            space += icon.getLeafNode() + getName();

            if(value != "") space += ": " + value;
            else space += " ";
            outBuffer.push_back(move(space));
        }
    }
    bool getIsLeaf() const override
    {
        return isLeaf;
    }
};

class Container : public Compose
{
private:
    bool isLeaf;

public:
    Container() : Compose("", 0), isLeaf(true) {}
    Container(string name, int level) : Compose(name, level), isLeaf(false) {}
    void draw(Icon &icon, Style &style, bool first, bool last, bool globalLast) const override
    {
        // Style format从前往后分别是：左边框0，右边框1，中间框2，连接线3, 横线4, 上连接5, 左上角6, 右上角7, 左下角8, 右下角9
        // 每个container画自己孩子
        if (level == 0) return;
        else
        {
            // 其他节点
            // format从前往后分别是：上下级连接0，右边框1，中间框2，连接线3, 横线4, 上连接5, 左上角6, 右上角7, 左下角8, 右下角9, 上下级连接靠左10
            // 左下角连接11,
            if(first){
                string space;
                for (int j = 0; j < level - 1; ++j)
                {
                    space += style.getFormat(6) + "  ";
                }
                space += style.getFormat(6) + style.getFormat(4);
                space += icon.getMiddleNode() + getName() + " ";
                outBuffer.push_back(move(space));
            }
            else if(last){
                string space;
                if(globalLast){
                    
                }
                else space += style.getFormat(3) + "  ";
                for (int j = 0; j < level - 2; ++j)
                {
                    space += style.getFormat(10) + "  ";
                }
                space += style.getFormat(0) + style.getFormat(4);
                space += icon.getMiddleNode() + getName() + " ";

                outBuffer.push_back(move(space));
            }
            else if(globalLast){
                string space;
                space += style.getFormat(8) + style.getFormat(11) + style.getFormat(11);
                // 上下级连接
                for(int j = 0; j < level - 2; ++j){
                    space += style.getFormat(11) + style.getFormat(11) + style.getFormat(11);
                }

                space += style.getFormat(5) + style.getFormat(4);
                space += icon.getMiddleNode() + getName() + " ";
                outBuffer.push_back(move(space));
            }
            else{
                string space;
                if(level != 1){
                    space += style.getFormat(3) + "  ";
                }
                for (int j = 0; j < level - 2; ++j)
                {
                    space += style.getFormat(10) +  "  ";
                }
                space += style.getFormat(2) + style.getFormat(4);
                space += icon.getMiddleNode() + getName() + " ";
                outBuffer.push_back(move(space));
            }
        }
    }

    bool getIsLeaf() const override
    {
        return isLeaf;
    }
};
#endif