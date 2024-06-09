#ifndef ITERATOR_H
#define ITERATOR_H
#include "Compose.h"
#include<nlohmann/json.hpp>

using nlohmann::json;

class Iterator
{
public:
    virtual ~Iterator() {}
    virtual std::shared_ptr<Compose> getNext() = 0;
    virtual bool hasNext() const = 0;
    virtual int getLen() = 0;
    virtual std::shared_ptr<Compose> getIndex(int i) = 0;
    virtual std::shared_ptr<Compose> getBack() = 0;
    virtual void reset() = 0;
};

class ComposeIterator : public Iterator
{
private:
    std::vector<std::shared_ptr<Compose>> inner;
    int len;
    int max_level; // 最深的儿子level
    int index;

public:
    ComposeIterator(std::vector<std::shared_ptr<Compose>> compose, int maxlen, int maxlevel) : inner(compose), 
                                                                                len(maxlen), max_level(maxlevel),
                                                                                   index(0) {}

    std::shared_ptr<Compose> getNext() {
        if (hasNext()) {
            index++;
            return inner[index - 1];
        }
        return nullptr;
    }

    bool hasNext() const override
    {
        return index < inner.size();
    }

    int getLen()
    {
        return inner.size();
    }

    std::shared_ptr<Compose> getIndex(int i){
        return inner[i];
    }

    std::shared_ptr<Compose> getBack()
    {
        return inner.back();
    }

    // 重置迭代器
    void reset(){
        index = 0;
    }
    
};

void parse_json(vector<shared_ptr<Compose>>& root,int &max_len, const json& j, const std::string& key, int level) {
    if (j.is_object()) {
        auto container = std::make_shared<Container>(key, level);
        root.push_back(container);
        for (auto it = j.begin(); it != j.end(); ++it) {
            parse_json(root, max_len, it.value(), it.key(), level+1);
        }
    } else {
        std::string val=j.dump();
        val = (val=="null")?"":val;
        int currnet_len=level;
        max_len = max_len < currnet_len ? currnet_len:max_len;
        root.push_back(std::make_shared<Leaf>(key, level, val));
    }
}
std::shared_ptr<Iterator> CreateIterator(const json* j){
    vector<shared_ptr<Compose>> root;
    int max_len = 0;
    // 转成所实现的组合模式
    parse_json(root, max_len, *j, "root", 0);
    root.erase(root.begin());
    return std::make_shared<ComposeIterator>(root, root.size(), max_len);
}

#endif