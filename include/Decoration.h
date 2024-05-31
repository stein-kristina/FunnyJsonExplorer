#pragma once
#ifndef DECORATION_H
#define DECORATION_H
#include<cassert>
using namespace std;
enum Type
{
    Icon_, Style_
};

class Decoration
{
public:
    virtual ~Decoration() {}
    virtual int getType() const = 0;
    virtual string getMiddleNode() const{ return "";};
    virtual string getLeafNode() const{ return ""; };
    virtual string getStyle() const{ return ""; };
    virtual void setFormat(vector<string> format){};
    virtual string getFormat(int idx){ return ""; };
};

class Icon : public Decoration
{
private:
    string middleNode, leafNode;
    int type;
public:
    Icon(string middleNode, string leafNode, int type) : middleNode(middleNode), leafNode(leafNode), type(type){}
    int getType() const override
    {
        return type;
    }
    string getMiddleNode() const
    {
        return middleNode;
    }
    string getLeafNode() const
    {
        return leafNode;
    }
};

class Style : public Decoration
{
private:
    string style;
    int type;
    vector<string> format; // 风格
public:
    Style(string style, int type) : style(style), type(type){}
    int getType() const override
    {
        return type;
    }
    string getStyle() const
    {
        return style;
    }
    void setFormat(vector<string> format)
    {
        // format从前往后分别是：上下级连接0，右边框1，中间框2，连接线3, 横线4, 上连接5, 左上角6, 右上角7, 左下角8, 右下角9, 上下级连接靠左10
        // 左下角连接11, 
        // tree: └, " ", ├, │, ─, " ", ├, " ", └, " ", " ", " "
        // rectangle: ├, ┤, ├, │, ─, ┴, ┌, ┐, └, ┘, │, ─
        this->format = format;
    }
    string getFormat(int idx){
        assert(idx < format.size() && idx >= 0);
        return format[idx];
    }
};

#endif