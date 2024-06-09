#ifndef STRATEGY_H
#define STRATEGY_H
#include "Iterator.h"
#include "Compose.h"
#include "Factory.h"

class Strategy
{
public:
    virtual ~Strategy() {}
    virtual void drawWithIcon(shared_ptr<Iterator> s,Icon &i) = 0;
    virtual void normalDraw(std::shared_ptr<Iterator> &iterator, Icon &icon, Style &style);
};  
// 绘图
void Strategy::normalDraw(shared_ptr<Iterator> &iterator, Icon &icon, Style &style)
{
    outBuffer.clear();
    // 需要统计level1的最后一个才行
    int isLevel1Last = -1;
    std::vector<bool> levelLast(iterator->getLen(), 0);
    int curlevel = iterator->getBack()->level;
    for(int i=iterator->getLen()-1; i>=2; --i){
        if(iterator->getIndex(i)->level > iterator->getIndex(i-1)->level){
            if(iterator->getIndex(i-1)->level < curlevel){
                // 后面比前面大的第一个点
                levelLast[i-1] = true;
                curlevel = iterator->getIndex(i-1)->level;
            }
        }

        if(iterator->getIndex(i)->level < iterator->getIndex(i-1)->level){
            levelLast[i-1] = true;
            curlevel = iterator->getIndex(i-1)->level;
        }
        if(iterator->getIndex(i)->level == 1){
            // 最后一个1
            isLevel1Last = isLevel1Last == -1 ? i : isLevel1Last;
        }  
    }

    
    for(int i=0; i < iterator->getLen(); i++){
        auto x = iterator->getNext();
        if(i == 0){
            if (iterator->getLen() == 1){
                x->draw(icon, style, false, true, false);
            }
            else{
                x -> draw(icon, style, true, false, false);
            }
        }
        else if(i == iterator->getLen()-1){
            x->draw(icon, style, false, false, true);
        }
        else{
            if(levelLast[i] == true){
                // 等级不相等时候才会出现last
                if(x->level == 1){
                    // level1的最后一个之前
                    if(i < isLevel1Last){
                        x->draw(icon, style, false, false, false);
                    }
                    else x->draw(icon, style, false, true, true);
                }
                else{
                    // if(root[i]->level != 1 && i < isLevel1Last){
                        
                    // }
                    // else x->draw(icon, style, false, true, false);
                    x->draw(icon, style, false, true, false);
                }
            }
            else{
                if (i > isLevel1Last){
                    // level1的最后一个紧接着的儿子
                    x->draw(icon, style, false, true, true);
                }
                else{
                    x->draw(icon, style, false, false, false);
                }
            }
        }
    }

    // 获取一行最长的字符串，准备填充
    int len = 0;
    for (auto &x : outBuffer)
    {
        // cout<<x.size()<<endl;
        len = max(len, (int)x.size());
    }
    // 填充
    // 额外填充6个横线
    int additional = 6;

    for (int i = 0; i < outBuffer.size(); ++i)
    {
        string &x = outBuffer[i];
        string output = x;

        int diff = (len-x.size());

        for (int i = 0; i < diff + additional; ++i)
        {
            output += style.getFormat(11);
        }
        if (i == 0)
        {
            // 第一行
            output += style.getFormat(7);
        }
        else if (i == outBuffer.size() - 1)
        {
            // 最后一行
            int mu = 6;
            if(iterator->getBack()->level == 1) mu=0;
            for (int i = 0; i < mu; ++i)
            {
                output+= style.getFormat(11);
            }
            output += style.getFormat(9);
        }
        else
        {
            output += style.getFormat(1);
        }
        cout << output << endl;
    }

}

class TreeStrategy : public Strategy
{
private:
    shared_ptr<Style> changeStyle;
public:
    TreeStrategy() {
        shared_ptr<TopAbstractFactory> factory(new MyFactory());
        shared_ptr<ProductFactory> styleFactory = factory->createStyleFactory();
        vector<string> treeFormat = {"└", " ", "├", "|", "─", "└", "├", " ", " ", " ", " ", " "};
        shared_ptr<Decoration> myStyle = styleFactory->createStyle("tree", treeFormat);
        changeStyle = dynamic_pointer_cast<Style>(myStyle);
    }
    // 以树状风格画图
    void drawWithIcon(shared_ptr<Iterator> iterator,Icon &icon){
        normalDraw(iterator, icon, *changeStyle);
    }
};

class RectangleStrategy : public Strategy
{
private:
    shared_ptr<Style> changeStyle;
public:
    RectangleStrategy() {
        shared_ptr<TopAbstractFactory> factory(new MyFactory());
        shared_ptr<ProductFactory> styleFactory = factory->createStyleFactory();
        vector<string> rectangleFormat = {"├", "┤", "├", "|", "─", "┴", "┌", "┐", "└", "┘", "|", "─"};
        shared_ptr<Decoration> myStyle = styleFactory->createStyle("rectangle", rectangleFormat);
        changeStyle = dynamic_pointer_cast<Style>(myStyle);
    }
    // 以树状风格画图
    void drawWithIcon(shared_ptr<Iterator> iterator,Icon &icon){
        normalDraw(iterator, icon, *changeStyle);
    }
};

#endif