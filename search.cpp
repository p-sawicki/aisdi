#include <vector>
#include <string>
std::vector<int> kmp_table(std::string input){
    std::vector<int> kmpt(input.length(), 0);
    kmpt[0] = -1;
    int cnd = 0;
    unsigned int i = 1;
    for(; i < input.length(); ++i){
        if(input[i] == input[cnd])
            kmpt[i] = kmpt[cnd];
        else{
            kmpt[i] = cnd;
            cnd = kmpt[cnd];
            while(cnd >= 0 && kmpt[i] != kmpt[cnd])
                cnd = kmpt[cnd];
        }
        ++cnd;
    }
    kmpt[i] = cnd;
    return std::move(kmpt);
}
std::vector<int> kmp_search(std::string input, std::string pattern){
    int posInput = 0;
    int posPattern = 0;
    std::vector<int> kmpt = kmp_table(input);
    std::vector<int> result;
    while(posInput < input.length()){
        if(pattern[posPattern] == input[posInput]){
            ++posInput;
            ++posPattern;
            if(posPattern == pattern.length()){
                result.push_back(posInput - posPattern);
                posPattern = kmpt[posPattern];
            }
        }
        else{
            posPattern = kmpt[posPattern];
            if(posPattern < 0){
                ++posInput;
                ++posPattern;
            }
        }
    }
    return std::move(result);
}

