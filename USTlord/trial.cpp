#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
using namespace std;

class Haha{
    public:
    enum TT{A,B,C};
    static bool cmp(Haha* aa, Haha* bb){
        if(aa->a<bb->a){return true;}
        return false;
    }
    void setTT(TT a){this->a=a;}
    TT getTT(){return a;}
    bool operator<(const Haha& ak){if(this->a<ak.a){return true;}return false;}
    private:
    TT a;
};

bool compareInt(int a,int b){
    return a>b;
}
void printVector(vector<int> vect){
    for (int i=0;i<vect.size();++i){
        cout << vect[i] << endl;
    }
}

int main(){
    //check whether the input of cards is valid
    int valid=false;
    while(!valid){
        cout << "Please enter the set of cards : " << endl;
        vector<string*> cards(0,nullptr);
        string card_name="";
        char temp='\0';

        while(true){
            temp=cin.get();
            if(temp=='\n'){
                if(card_name.size()==2){
                    cards.push_back(new string(card_name));
                    card_name="";
                }
                else if(card_name.size()!=0){
                    cout << "Invalid input." << endl;
                    for(int i=0;i<cards.size();++i){
                        delete cards[i];
                    }
                    cards.clear();
                    break;
                }
                break;
            }
            else if(temp==' '){
                if(card_name.size()==2){
                    cards.push_back(new string(card_name));
                    card_name="";
                }
                else if(card_name.size()!=0){
                    cout << "Invalid input." << endl;
                    for(int i=0;i<cards.size();++i){
                        delete cards[i];
                    }
                    cards.clear();
                    break;
                }
                continue;
            }
            else{
                card_name+=temp;
            }
        }
        if(cards.size()>0){
            valid=true;
            for(int i=0;i<cards.size();i++){
                cout << *cards[i] << endl;
            }
        }
        if(temp!='\n'){
            cin.ignore(999,'\n');
        }
    }
    
    
    

    // string a;
    // a+=cin.get();
    // cout << a;
    
    //string input;
    //cin >> input;
    //cout << input;
    // char input[100]{};

    // cin.getline(input,999,'\n');
    // for(int i=0;i<100;i++){
    //     cout << input[i];
    // }

    // for(int i=0;i<10;i++){
    //     for(int j=0;j<10;j++){
    //         cout << "A\t";
    //     }
    // }

    // string a="sadf"+to_string(1);
    // cout << a << endl;
    // vector<int> b(10,0);
    // b[2]=3;
    // cout << b[2] << endl;
    // vector<vector<int>*> vv;
    // vv.push_back(new vector<int>(11,2));
    // vv.push_back(new vector<int>(12,1));
    // for(int i=0;i<vv.size();++i){
    //     for(int j=0;j<vv[i]->size();++j){
    //         cout << (*vv[i])[j] << '\t';
    //     }
    // }
    // //vector<const Card*> my_cards={new Card('s','T'),new Card('c','2'),new Card('s','2'),new Card('d','2'),new Card('h','T')};
    // vector<const Card*> my_cards={new Card('s','4'),new Card('c','3'),new Card('s','4'),new Card('d','3'),new Card('h','4'),new Card('h','4')};
    // vector<const Card*> op_cards={new Card('s','3'),new Card('c','4'),new Card('s','3'),new Card('d','4'),new Card('d','4'),new Card('d','4')};
    // //vector<const Card*> my_cards={new Card('r','W'),new Card('b','W')};
    // // for(int i=0;i<4;i++){
    // //     my_cards[i]=new Card('s',figures_to_int[4]);
    // //     //my_cards[i+5]=new Card('h',figures_to_int[i+1]);        
    // // }
    // CardsGroup a(my_cards);
    // CardsGroup b(op_cards);
    // cout << boolalpha << a.is_valid() << endl;
    // cout << "check_continuous" << endl;
    // cout << CardsGroup::check_continuous(a.get_figures_distribution(),2) << endl;
    // cout << a.get_reference_card()->get_string() << endl;
    // cout << a.get_cards_type().to_string() << endl;
    // cout << a.get_cards_type().get_num_cards() << endl;
    // cout << a.get_cards_type().get_max_repeat_times() << endl;
    // vector<int> fig_dis=a.get_figures_distribution();
    // for(int i=0;i<NUMBER_OF_FIGURES;i++){
    //     cout << fig_dis[i] << '\t';
    // }
    // cout << endl;
    // cout << CardsGroup::count_max_continuous_times(a.get_figures_distribution(),2) << endl;

    // cout << a.compare(b) << endl;
    // cout << b.compare(a) << endl;

    // CardsType a(CardsType::Type::FOUR_WITH_TWO,6,4,false);
    // cout << a.to_string() << endl;
    // CardsType b(CardsType::Type::FOUR_WITH_TWO,6,4,false);
    // cout << boolalpha;
    // cout << CardsType::is_equal(a,b) << endl;
    // CardsType c(CardsType::Type::TRIO_CONTINUOUS,9,3,true);
    // cout << c.to_string() << endl;
    // cout << CardsType::is_comparable(b,c) << endl;
    // CardsType d(CardsType::Type::BOMB,4,4,false);
    // cout << CardsType::is_comparable(c,d) << endl;


    // Card a('s','5');//SPADE 5
    // Card b(Card::Color::DIAMOND,0);//DIAMOND 3
    // Card c(Card::Color::SPADE,0);//SPADE 3
    // Card d(Card::Color::BLACK_JOKER,NUMBER_OF_FIGURES-1);//BLACK_JOKER W
    // Card e(Card::Color::RED_JOKER,NUMBER_OF_FIGURES-1);//RED_JOKER W
    // cout << boolalpha;
    // cout << (a<b) << endl;
    // cout << (b<=c) << endl;
    // cout << (b==c) << endl;
    // cout << (d<a) << endl;
    // cout << (b!=c) << endl;
    // cout << (d<e) << endl;
    // a.print();
    // e.print();
    // cout << d.get_string() << endl;
    // cout << b.get_string() << endl;

    // cout << "testing static compare\n";
    // Card* aPtr=new Card(a);
    // Card* bPtr=new Card(b);
    // Card* cPtr=new Card(c);
    // cout << Card::compare_value(aPtr,bPtr) << endl;
    // cout << Card::compare_value(bPtr,cPtr) << endl;
    // cout << Card::strictly_compare(bPtr,cPtr) << endl;
    // for(int i=0;i<15;++i){
    //     continue;
    // }

    // string a{'a','b'};
    // a+="as";
    // a+='a';
    // cout << a << endl;
    // a.erase(a.begin()+1);
    // cout << a << endl;

    // vector<int> vv{0,1,2,3,0,1,0,2,0,12,3};
    // cout << vv.size() << endl;
    // vv.erase(vv.begin()+2);
    // cout << vv.size() << endl;
    // //vv.clear();
    // //cout << vv.size() << endl;
    // for (int i=0;i<vv.size();++i){
    //     cout << vv[i] << '\t';
    // }
    // cout << endl;

    // for(int i=0;i<vv.size();++i){
	// 	if(vv[i]==0){
	// 		vv.erase(vv.begin()+i);
	// 		--i;
	// 	}
	// }
    // for (int i=0;i<vv.size();++i){
    //     cout << vv[i] << '\t';
    // }
    // cout << endl;

    // vector<int> gg=vv;
    // gg[1]=32;
    // for (int i=0;i<vv.size();++i){
    //     cout << vv[i] << '\t';
    // }
    // cout << endl;
    // for (int i=0;i<gg.size();++i){
    //     cout << gg[i] << '\t';
    // }
    // cout << endl;

    // sort(gg.begin(),gg.end(),std::greater<int>());
    // for (int i=0;i<gg.size();++i){
    //     cout << gg[i] << '\t';
    // }
    // cout << endl;


    // vector<Haha*> lis(10,nullptr);
    // for(int i=0;i<lis.size();i++){lis[i]=new Haha();}
    // for(int i=0;i<lis.size();i++){
    //     if(i%3==0){lis[i]->setTT(Haha::A);}
    //     else if(i%3==1){lis[i]->setTT(Haha::B);}
    //     else if(i%3==2){lis[i]->setTT(Haha::C);}
    // }
    // sort(lis.begin(),lis.end(),Haha::cmp);
    // Haha yy;cout << boolalpha;
    // for(int i=0;i<lis.size();++i){
    //     cout << lis[i]->getTT() << endl;
    //     cout << (yy<*lis[i]) << endl;
    // }
    
    // srand(time(0));
    // cout << rand() << endl;
    // vector<const int*> aa(10,nullptr);
    // cout << aa.size() << endl;
    // for(int i=0;i<aa.size();++i){
    //   aa[i]=new int{i};  
    // }
    // for(int i=0;i<aa.size();++i){
    //     cout << *aa[i] << endl;
    // }
    // vector<int> ak(5,1);
    // vector<int> bk(10,2);
    // ak=bk;
    // printVector(ak);

    //cout << aa.size() << endl;
    // vector<int> a(5,23);
    // cout << a.size() << endl;
    // a.resize(12);
    // cout << a.size() << endl;
    // for(int i=0;i<a.size();++i){
    //     cout << a[i] << endl;
    // }
    // for(int i=0;i<a.size();++i){
    //     a[i]=rand()%100;
    // }
    // sort(a.begin(),a.end(),compareInt);
    // for(int i=0;i<a.size();++i){
    //     cout << a[i] << endl;
    // }
    // cout << a.front() << endl;
    // cout << a.back() << endl;

    // a.push_back(234);
    // a.push_back(123);
    // printVector(a);
    // a.pop_back();
    // printVector(a);
    // cout << boolalpha << a.empty() << endl;
    // a.insert(a.begin()+3,{1,2,3});
    // cout << "After insert"<< endl;
    // printVector(a);
    // vector<int> b={9,8,7,6,5};
    // a.insert(a.end(),b.begin()+1,b.end()-1);
    // printVector(a);
    return 0;
}