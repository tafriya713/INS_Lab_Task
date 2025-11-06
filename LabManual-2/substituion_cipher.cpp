#include<iostream>
#include<string>
#include<map>
#include<vector>
#include <algorithm>   
#include <cctype>

using namespace std;
  

string cleanText(const string &text){
    string cleaned;
    for( char ch : text){
        if(isalpha(ch))
        cleaned += tolower(ch);
    }
    return cleaned;
}

map<char,int> getFrequency(const string &text){
    map<char,int> freq;
    for(char ch : text){
        if (isalpha(ch))
        freq[towlower(ch)]++;
    }
    return freq;
}
map<char ,char> createMapping(map<char,int> &freq){
    string englishFreqOrder ="etaoinshrdlcumwfgypbvkjxqz";

    vector<pair <char,int>> sortedFreq(freq.begin(),freq.end());
    sort(sortedFreq.begin(), sortedFreq.end(),
     [](auto &a, auto &b) { return a.second > b.second; });
     map<char, char> mapping;
    for (size_t i = 0; i < sortedFreq.size() && i < englishFreqOrder.size(); i++) {
        mapping[sortedFreq[i].first] = englishFreqOrder[i];
    }
    return mapping;
}

string decrypt(const string &cipher, map<char, char> &mapping) {
    string result;
    for (char ch : cipher) {
        if (isalpha(ch)) {
            char lower = tolower(ch);
            if (mapping.find(lower) != mapping.end())
                result += mapping[lower];
            else
                result += ch;
        } else {
            result += ch; // keep punctuation
        }
    }
    return result;
}
int main(){
    string cipher1 = R"(af p xpkcaqvnpk pfg, af ipqe qpri, gauuikifc tpw, ceiri udvk tiki afgarxifrphni cd eao-
wvmd popkwn, hiqpvri du ear jvaql vfgikrcpfgafm du cei xkafqaxnir du xrwqedearcdkw pfg
du ear aopmafpcasi xkdhafmr afcd fit pkipr. ac tpr qdoudkcafm cd lfdt cepc au pfwceafm
epxxifig cd ringdf eaorinu hiudki cei opceiopcaqr du cei uaing qdvng hi qdoxnicinw tdklig dvc-
pfg edt rndtnw ac xkdqiigig, pfg edt odvfcpafdvr cei dhrcpqnir--ceiki tdvng pc niprc kiopaf dfi
mddg oafg cepc tdvng qdfcafvi cei kiripkqe)";

string cipher2 = R"(aceah toz puvg vcdl omj puvg yudqecov, omj loj auum klu thmjuv hs klu zlcvu shv
zcbkg guovz, upuv zcmdu lcz vuwovroaeu jczoyyuovomdu omj qmubyudkuj vukqvm. klu
vcdluz lu loj avhqnlk aodr svhw lcz kvopuez loj mht audhwu o ehdoe eunumj, omj ck toz
yhyqeoveg auecupuj, tlokupuv klu hej sher wcnlk zog, klok klu lcee ok aon umj toz sqee hs
kqmmuez zkqssuj tckl kvuozqvu.)";

//clean and count frequency
auto freq1 = getFrequency(cipher1);
auto freq2 = getFrequency(cipher2);


//frequency mapping
 auto map1 = createMapping(freq1);
 auto map2 = createMapping(freq2);

 //decrypt
 string plain1 = decrypt(cipher1, map1);
 string plain2 = decrypt(cipher2, map2);

 cout << "---------- Cipher 1 --------\n";
    cout << plain1 << "\n\n";

    cout << " ----------Cipher 2 ---------------\n";
    cout << plain2 << "\n\n";


}


 
