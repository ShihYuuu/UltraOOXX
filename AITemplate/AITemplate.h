#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <ctime>
#include <set>
#include <random>
#include<vector>
#include<chrono>
#include<thread>
#include<cstdlib>
bool full(TA::UltraBoard input , int block){
    int x = block/3;
    int y = block%3;
    return (input.sub(x,y)).full();
}

std::pair<int,int> empty_place(TA::UltraBoard input , int loc , int next_block){
    int x = loc/3;
    int y = loc%3;
    if(full(input,next_block)){
        next_block = 8-loc;
    }
    int n_x = next_block/3;
    int n_y = next_block%3;
    n_x *= 3;
    n_y *= 3;
    if(input.get(n_x + x , n_y + y) != TA::BoardInterface::Tag::None){
        x = (8-loc)/3;
        y = (8-loc)%3;
    } 
    std::cout << n_x + x << " " << n_y + y << "\n";
    return std::make_pair(n_x + x , n_y + y);
}

class AI : public AIInterface
{
public:
    void init(bool order) override
    {
        first = order;
        now_dir = 4;
        next_block = 4;
        Main_x=-1;
        Main_y=-1;
        k=0;
        m=0;
    }

    void callbackReportEnemy(int x, int y) override
    {
        std::cout << x << " : " << y << "\n";
        x = x%3;
        y = y%3;
        next_block = x*3 + y ;
        this->Main_x=x%3;
        this->Main_y=y%3;
    }
    std::pair<int,int> queryWhereToPut(TA::UltraBoard input) override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(first){
            if(full(input , now_dir) ==  true && !full(input,next_block)){
                if(now_dir == 4) now_dir = next_block;
                else{
                    now_dir = 8 - now_dir;
                }
            }
            return empty_place(input,now_dir,next_block);
        }
        else{
            auto pair=select_random_pair(input);//std::make_pair(k,m);
            k++;
            m++;
            //select_ramdom_pair(MainBoard);
            return pair;
        }
    }
private:
    std::pair<int,int>select_random_pair(TA::UltraBoard MainBoard){
        srand(time(NULL));
        std::pair<int,int> random_pair;
        std::set<std::pair<int,int>> candidates;
        std::set<std::pair<int,int>> prioritycan;
        std::set<std::pair<int,int>> secondcan;
        
            if((Main_x==-1&&Main_y==-1)||MainBoard.sub(Main_x,Main_y).full()){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    
                            //if(ff==0){
                            if(MainBoard.state(i,j)!=TA::BoardInterface::Tag::None)continue;
                            if(((MainBoard.state(((i+1)%3),j))==TA::BoardInterface::Tag::X) &&
                            ((MainBoard.state(((i+2)%3),j))==TA::BoardInterface::Tag::X)){
                            int x=i*3;
                            int y=j*3;
                            for(int a=x;a<x+3;++a){
                            for(int b=y;b<y+3;++b){
                            if( MainBoard.get(a,b) == TA::BoardInterface::Tag::None ){
                            //candidates.insert(std::make_pair(a,b));
                            //if(ff==0){
                            if(((MainBoard.get(((a-x+1)%3+x),b))==TA::BoardInterface::Tag::X) &&
                            ((MainBoard.get(((a-x+2)%3+x),b))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(a,b));
                            //ff=1;
                            }
                            if(((MainBoard.get(a,((b-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(a,((b-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(a,b));
                            //ff=1;
                            }
                            if(((a-x!=1)&&(b-y!=1))||((a-x==1)&&(b-y==1))){
                            if(((MainBoard.get(((a-x+1)%3+x),((b-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(((a-x+2)%3+x),((b-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(i,j));
                            //ff=1;
                            }
                            }

                        
                            }
                    
                            }
                            }
                            //ff=1;
                        }
                        else if(((MainBoard.state(i,((j+1)%3)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.state(i,((j+2)%3)))==TA::BoardInterface::Tag::X)){
                            int x=i*3;
                            int y=j*3;
                            for(int a=x;a<x+3;++a){
                            for(int b=y;b<y+3;++b){
                            if( MainBoard.get(a,b) == TA::BoardInterface::Tag::None ){
                            //candidates.insert(std::make_pair(a,b));
                            //if(ff==0){
                            if(((MainBoard.get(((a-x+1)%3+x),b))==TA::BoardInterface::Tag::X) &&
                            ((MainBoard.get(((a-x+2)%3+x),b))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(a,b));
                            //ff=1;
                            }
                            if(((MainBoard.get(a,((b-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(a,((b-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(a,b));
                            //ff=1;
                            }
                            if(((a-x!=1)&&(b-y!=1))||((a-x==1)&&(b-y==1))){
                            if(((MainBoard.get(((a-x+1)%3+x),((b-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(((a-x+2)%3+x),((b-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(i,j));
                            //ff=1;
                            }
                            }

                        
                            }
                    
                            }
                            }
                            //ff=1;
                        }
                        
                        else if(((MainBoard.state(((i+1)%3),((j+1)%3)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.state(((i+2)%3),((j+2)%3)))==TA::BoardInterface::Tag::X)){
                            if(((i!=1)&&(j!=1))||((i==1)&&(j==1))){
                                int x=i*3;
                            int y=j*3;
                            for(int a=x;a<x+3;++a){
                            for(int b=y;b<y+3;++b){
                            if( MainBoard.get(a,b) == TA::BoardInterface::Tag::None ){
                            //candidates.insert(std::make_pair(a,b));
                            //if(ff==0){
                            if(((MainBoard.get(((a-x+1)%3+x),b))==TA::BoardInterface::Tag::X) &&
                            ((MainBoard.get(((a-x+2)%3+x),b))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(a,b));
                            //ff=1;
                            }
                            if(((MainBoard.get(a,((b-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(a,((b-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(a,b));
                            //ff=1;
                            }
                            if(((a-x!=1)&&(b-y!=1))||((a-x==1)&&(b-y==1))){
                            if(((MainBoard.get(((a-x+1)%3+x),((b-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(((a-x+2)%3+x),((b-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(i,j));
                            //ff=1;
                            }
                            }

                        
                            }
                    
                            }
                            }
                            //ff=1;
                        }
                        }
                }
            }
            for(int i=0;i<9;++i){
                for(int j=0;j<9;++j){
                    if(MainBoard.get(i,j)==TA::BoardInterface::Tag::None){
                      candidates.insert(std::make_pair(i,j));
                      if(((i%3==1)&&(j%3==1))&&prioritycan.empty()){
                          prioritycan.insert(std::make_pair(i,j));
                      }
                     }
                }
            }
            int n=rand()%candidates.size();
            if(prioritycan.empty()){
                auto it=candidates.begin();
                std::advance(it,n);
                random_pair=*it;
            }else{
                auto it=prioritycan.begin();
                random_pair=*it;
            }
        }else{
            int x=Main_x*3;
            int y=Main_y*3;
            //int ff=0;
            //int fs=0;
            for(int i=x;i<x+3;++i){
                for(int j=y;j<y+3;++j){
                        if( MainBoard.get(i,j) == TA::BoardInterface::Tag::None ){
                        candidates.insert(std::make_pair(i,j));
                        //if(ff==0){
                            if(((MainBoard.get(((i-x+1)%3+x),j))==TA::BoardInterface::Tag::X) &&
                            ((MainBoard.get(((i-x+2)%3+x),j))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(i,j));
                            //ff=1;
                        }
                        if(((MainBoard.get(i,((j-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(i,((j-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(i,j));
                            //ff=1;
                        }
                        if(((i-x!=1)&&(j-y!=1))||((i-x==1)&&(j-y==1))){
                            if(((MainBoard.get(((i-x+1)%3+x),((j-y+1)%3+y)))==TA::BoardInterface::Tag::X)&&
                            ((MainBoard.get(((i-x+2)%3+x),((j-y+2)%3+y)))==TA::BoardInterface::Tag::X)){
                            prioritycan.insert(std::make_pair(i,j));
                            //ff=1;
                        }
                        }
                        
                        //}
                        //if(fs==0){

                        //}
                        if(((MainBoard.get(((i-x+1)%3+x),j))==TA::BoardInterface::Tag::O) &&
                        ((MainBoard.get(((i-x+2)%3+x),j))==TA::BoardInterface::Tag::O)){
                            secondcan.insert(std::make_pair(i,j));
                            //fs=1;
                        }
                        if(((MainBoard.get(i,((j-y+1)%3+y)))==TA::BoardInterface::Tag::O)&&
                        ((MainBoard.get(i,((j-y+2)%3+y)))==TA::BoardInterface::Tag::O)){
                            secondcan.insert(std::make_pair(i,j));
                            //fs=1;
                        }
                        if(((i-x!=1)&&(j-y!=1))||((i-x==1)&&(j-y==1))){
                            if(((MainBoard.get(((i-x+1)%3+x),((j-y+1)%3+y)))==TA::BoardInterface::Tag::O)&&
                            ((MainBoard.get(((i-x+2)%3+x),((j-y+2)%3+y)))==TA::BoardInterface::Tag::O)){
                            secondcan.insert(std::make_pair(i,j));
                            //fs=1;
                        }
                        }
                        
                    }
                    
                }
            }
            int n=rand()%candidates.size();
            if(prioritycan.empty()){
                if(secondcan.empty()){
                    auto it=candidates.begin();
                    std::advance(it,n);
                    random_pair=*it;
                }else{
                    auto it=secondcan.begin();
                    random_pair=*it;
                }
                
            }else{
                auto it=prioritycan.begin();
                random_pair=*it;
            }
            
        }
        
        
        candidates.clear();
        prioritycan.clear();
        return random_pair;
        
    }
    int Main_x;
    int Main_y;
    int k;
    int m;
    int now_dir,next_block;
    bool first;
};
