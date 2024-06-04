#ifndef JSONEXPLORER
#define JSONEXPLORER
#include "Factory.h"
#include <regex>
#include"Compose.h"
#include<nlohmann/json.hpp>
using nlohmann::json;

class JsonExplorer
{
private:
    shared_ptr<Compose> root;
    shared_ptr<TopAbstractFactory> factory;
    shared_ptr<ProductFactory> decorationFactory;
    int max_len = 0;

    std::string preprocess_json(const std::string& raw_json) {
        // 正则表达式：匹配未加引号的属性名，并添加双引号
        std::regex re_unquoted(R"((\s*)(\w+)(\s*):)");
        std::string result = std::regex_replace(raw_json, re_unquoted, R"($1"$2"$3:)");

        // 正则表达式：匹配单引号，并替换为双引号
        std::regex re_single_quotes(R"('([^']*)')");
        result = std::regex_replace(result, re_single_quotes, R"("$1")");

        return result;
    }
    shared_ptr<Compose> parse_json(const json& j, const std::string& key, int level) {
        if (j.is_object()) {
            auto container = std::make_shared<Container>(key, level);
            for (auto it = j.begin(); it != j.end(); ++it) {
                container->add(parse_json(it.value(), it.key(), level+1));
            }
            return container;
        } else {
            std::string val=j.dump();
            val = (val=="null")?"":val;
            int currnet_len=level;
            max_len = max_len < currnet_len ? currnet_len:max_len;
            return std::make_shared<Leaf>(key, level, val);
        }
    }


public:
    JsonExplorer(string style, string icon_family)
    {
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
        // vector<string> fileContent;
        // getFileContent(fileContent, json_file);
        // int index = 1;
        // fileContent.pop_back();

        // 转成所实现的组合模式
        root = parse_json(j, "root", 0);
    }

    // 设置客户自定义工厂
    void setFactory(shared_ptr<TopAbstractFactory> factory)
    {
        this->factory = move(factory);
    }

    // 建立图标和风格工厂
    shared_ptr<ProductFactory> createIconFactory()
    {
        return factory->createIconFactory();
    }
    shared_ptr<ProductFactory> createStyleFactory()
    {
        return factory->createStyleFactory();
    }

    // 设置装饰工厂
    void setDecorationFactory(shared_ptr<ProductFactory> decorationFactory)
    {
        this->decorationFactory = decorationFactory;
    }

    // 生产产品
    shared_ptr<Decoration> createIcon(string middleNode, string leafNode)
    {
        return decorationFactory->createIcon(middleNode, leafNode);
    }
    shared_ptr<Decoration> createStyle(string style, vector<string> &format)
    {
        return decorationFactory->createStyle(style, format);
    }

    // 绘图
    void draw(Icon &icon, Style &style) const
    {
        outBuffer.clear();
        root->draw(icon, style, false);

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
                for (int i = 0; i < 6; ++i)
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

        // cout << outBuffer.back().size() << " " << len << " " << endl;
    }
};
#endif