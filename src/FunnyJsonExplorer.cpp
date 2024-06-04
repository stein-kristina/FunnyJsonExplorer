#include <fstream>
#include <locale>
#include <codecvt>
#include "../include/Compose.h"
#include "../include/Factory.h"
#include "../include/JsonExplorer.h"

int main(int argc, char *argv[])
{
    std::string json_file = "C:\\Users\\Amadeus\\Desktop\\program\\Funny_JSON_Explorer\\input/input2.json";
    std::string style = "tree";
    std::string icon_family = "default";

    // 检查参数数量
    if (argc != 7)
    {
        std::cerr << "Usage: " << argv[0] << " -f <json file> -s <style> -i <icon family>" << std::endl;
        return 1;
    }

    // 逐个分析参数
    for (int i = 1; i < argc; i += 2)
    {
        std::string arg = argv[i];

        // 判断参数
        if (arg == "-f")
        {
            json_file = argv[i + 1];
        }
        else if (arg == "-s")
        {
            style = argv[i + 1];
        }
        else if (arg == "-i")
        {
            icon_family = argv[i + 1];
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    // 输出参数值，以确认程序正确接收
    
    string init;
    init += "| JSON file: " + json_file + " | ";
    init +=  " Style: " + style + " | ";
    init += "Icon family: " + icon_family  + " | ";
    cout<< string(init.size(), '-')<<endl;
    cout<<init<<endl;
    cout<< string(init.size(), '-')<<endl;

    setlocale(LC_ALL, "chs");
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // 创建JsonExplorer对象
    JsonExplorer explorer(style, icon_family);

    // 加载json文件
    explorer._load(json_file);

    // 建立客户工厂
    MyFactory factory;
    explorer.setFactory(make_shared<MyFactory>(factory));

    // 建立装饰工厂
    shared_ptr<ProductFactory> iconFactory = explorer.getProductFactory()->createIconFactory();
    shared_ptr<ProductFactory> styleFactory = explorer.getProductFactory()->createStyleFactory();

    // 设置装饰工厂
    explorer.setDecorationFactory(iconFactory);
    shared_ptr<Decoration> myIcon = nullptr; 
    vector<string> icons(2);
    if(icon_family == "default"){
        icons[0] = " ";
        icons[1] = " ";
    }
    else if(icon_family == "poker"){
        icons[0] = converter.to_bytes(wstring(L"♢"));
        icons[1] = converter.to_bytes(wstring(L"♤"));
    }
    else{
        cout<<"Unknown icon family"<<endl;
        return 1;
    }
    myIcon = explorer.getDecorationFactory()->createIcon(icons[0], icons[1]);

    // format从前往后分别是：上下级连接0，右边框1，中间框2，连接线3, 横线4, 上连接5, 左上角6, 右上角7, 左下角8, 右下角9, 上下级连接靠左10
    // 左下角连接11,
    vector<string> format;
    shared_ptr<Decoration> myStyle = nullptr;
    explorer.setDecorationFactory(styleFactory);
    if(style == "tree"){
        format = {"└", " ", "├", "|", "─", "└", "├", " ", " ", " ", " ", " "};
    }
    else if (style == "rectangle"){
        format = {"├", "┤", "├", "|", "─", "┴", "┌", "┐", "└", "┘", "|", "─"};
    }
    else{
        cout<<"Unknown style"<<endl;
        return 1;
    }
    myStyle = explorer.getDecorationFactory()->createStyle(style, format);

    // 绘图
    Icon* changeIcon = dynamic_cast<Icon*>(myIcon.get());
    Style* changeStyle = dynamic_cast<Style*>(myStyle.get());
    explorer.draw(*changeIcon, *changeStyle);

    return 0;
}
