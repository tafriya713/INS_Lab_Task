#include <iostream>
#include<vector>
#include<map>
#include <iomanip>
using namespace std;

vector<string> common_words = {
    "the","and","to","of","a","in","is","that","was","for","it","with","as","on","this","be"
};

bool has_vowel(const string &s){
    for(char ch : s){
        char c = tolower(ch);
        if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u') return true;
    }
    return false;
}

vector<string> tokenize(const string &s){
    vector<string> out;
    string w;
    for(char ch : s){
        if(isalpha(ch) || ch=='\'') w += ch;
        else {
            if(!w.empty()){
                out.push_back(w);
                w.clear();
            }
        }
    }
    if(!w.empty()) out.push_back(w);
    return out;
}

int count_common_words(const string &text){
    string lower = text;
    for(char &c : lower) c = tolower(c);
    int cnt = 0;
    for(const string &w : common_words){
        size_t pos = 0;
        while(true){
            pos = lower.find(w, pos);
            if(pos==string::npos) break;
            // whole-word match
            bool left_ok = (pos==0) || !isalpha(lower[pos-1]);
            bool right_ok = (pos + w.size() >= lower.size()) || !isalpha(lower[pos + w.size()]);
            if(left_ok && right_ok) cnt++;
            pos = pos + w.size();
        }
    }
    return cnt;
}

double readable_token_ratio(const string &text){
    auto toks = tokenize(text);
    if(toks.empty()) return 0.0;
    int likely = 0;
    for(auto &t : toks){
        if((int)t.size() >= 2 && has_vowel(t)) likely++;
    }
    return (double)likely / (double)toks.size();
}

int distinct_mapped_letters(const string &decrypted){
    // count  lowercase letters present in decrypted 
    vector<bool> present(26,false);
    for(char ch : decrypted){
        if(isalpha(ch)){
            present[tolower(ch)-'a'] = true;
        }
    }
    int cnt = 0;
    for(bool b : present) if(b) cnt++;
    return cnt;
}

double compute_score(const string &plain){
    //  common words (w1), readable ratio (w2), mapped letters (w3)
    double w1 = 2.0, w2 = 5.0, w3 = 1.0;
    int cw = count_common_words(plain);
    double rratio = readable_token_ratio(plain);
    int mapped = distinct_mapped_letters(plain);
    
    double mapped_norm = mapped / 26.0;
    double score = w1 * cw + w2 * rratio + w3 * mapped_norm;
    return score;
}

int main(){
    //got this decryption from substitution_cipher.cpp
    string plain1 = R"(in o godaiculod onr, in eoch cose, rimmedena woy, ahese mtud wede inrisgensoble at hif-
yupt ofodyl, becouse tm his juicv unredsaonrinp tm ahe gdincigles tm gsychthisatdy onr)";
    string plain2 = R"(cdncl fts yeag aduo trh yeag peumndta, trh oth ceer ioe flrhea lw ioe sodae wla
sdjig getas, eyea sdrue ods aevtaktcne hdstppetatrue trh mrejpeuieh aeimar. ioe
aduoes oe oth calmboi ctuk walv ods iatyens oth rlf ceulve t nlutn neberh, trh di fts
plpmntang cendeyeh, fotieyea ioe lnh wlnk vdboi stg, ioti ioe odnn ti ctb erh fts wmnn lw
imrrens simwweh fdio iaetsmae.)";

    double s1 = compute_score(plain1);
    double s2 = compute_score(plain2);

    cout << fixed << setprecision(4);
    cout << "Cipher-1 score = " << s1 << "\n";
    cout << "  common-word-count = " << count_common_words(plain1) 
         << ", readable-token-ratio = " << readable_token_ratio(plain1)
         << ", distinct-mapped-letters = " << distinct_mapped_letters(plain1) << "\n\n";

    cout << "Cipher-2 score = " << s2 << "\n";
    cout << "  common-word-count = " << count_common_words(plain2) 
         << ", readable-token-ratio = " << readable_token_ratio(plain2)
         << ", distinct-mapped-letters = " << distinct_mapped_letters(plain2) << "\n\n";

    if(s1 > s2) cout << "=> Verdict: Cipher-1 is easier to break (higher score)\n";
    else if(s2 > s1) cout << "=> Verdict: Cipher-2 is easier to break (higher score)\n";
    else cout << "=> Verdict: about equal difficulty\n";

    return 0;
}
