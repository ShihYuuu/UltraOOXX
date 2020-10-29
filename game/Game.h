#pragma once

#include <UltraOOXX/UltraBoard.h>
#include <UltraOOXX/Wrapper/AI.h>
#include <GUI/GUIInterface.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>

namespace TA
{
    class UltraOOXX
    {
    public:
        int win,win_2,win_3;
        UltraOOXX(
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(1000)
        ):
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr),
            win(0),
            win_2(0),
            win_3(0),
            last(-87,-87),
            MainBoard()
        {
            gui = new ASCII;
        }

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

        void run()
        {
            gui->title();
            int round = 0;
            if( !prepareState() ) return ;

            //Todo: Play Game
            updateGuiGame();
            AIInterface *first = m_P2;
            AIInterface *second = m_P1;
            AIInterface *tmp = nullptr;
            BoardInterface::Tag tag = BoardInterface::Tag::X;

            while (!checkGameover()&&round != 81) {          
                round++;
                if(tag == BoardInterface::Tag::X) tag = BoardInterface::Tag::O;
                else tag = BoardInterface::Tag::X;
                tmp = first;
                first = second;
                second = tmp;

                if (!playOneRound(first, tag, second)) {
                    if(tag == BoardInterface::Tag::O)
                        putToGui("Player 1 lose\n");
                    else if(tag == BoardInterface::Tag::X)
                        putToGui("Player 2 lose\n");
                    break;
                }
                putToGui("round = %d\n", round);
                updateGuiGame();
                //break;
            }
            if(round == 81) std::cout << "It's Tie !\n";
        } 

   private:
        void updateGuiGame()
        {
            gui->updateGame(MainBoard);
        }

        bool playOneRound(AIInterface *user, BoardInterface::Tag tag, AIInterface *enemy)
        {
            auto pos = call(&AIInterface::queryWhereToPut, user, MainBoard);
            int x = pos.first;
            int y = pos.second;
            putToGui("x , y = %d , %d\n", x, y);
            if(last.first != -87 || last.second != -87){
                if(x < 0 || x > 8 || y < 0 || y > 8) return false;
                int l_x = last.first;
                int l_y = last.second;
                l_x %= 3;
                l_y %= 3;
                if(!(MainBoard.sub(l_x,l_y)).full()){
                    if(x < l_x*3 || x > l_x*3 + 2 || y < l_y*3 || y > l_y*3 +2) return false;
                }
                if(MainBoard.get(x,y) != BoardInterface::Tag::None) return false;
            }
            last.first = x;
            last.second = y;
            
            BoardInterface::Tag &t = MainBoard.get(x, y);
            t = tag;
            /*
            if( user->x()!= -1 && /*!MainBoard.sub(x/3,y/3).full() &&
             ((x/3) != user->x() || (y/3) != user->y())) 
                return false;
            */
             //ckeck if this make a line
            makeLine(MainBoard.sub(x/3, y/3), x%3, y%3, tag);
            call(&AIInterface::callbackReportEnemy, enemy, x, y);
            return true;
        }
        void makeLine(Board &b, int x, int y, BoardInterface::Tag tag)
        {
            if(b.getWinTag() != BoardInterface::Tag::None) return;

            BoardInterface::Tag result = BoardInterface::Tag::None;
            BoardInterface::Tag t1, t2, t3, t4, t5, t6, t7, t8, t9;
            t1 = b.state(0, 0);
            t2 = b.state(0, 1);
            t3 = b.state(0, 2);
            t4 = b.state(1, 0);
            t5 = b.state(1, 1);
            t6 = b.state(1, 2);
            t7 = b.state(2, 0);
            t8 = b.state(2, 1);
            t9 = b.state(2, 2);
            if( (t1 == t2) && (t2 == t3) && (t1!=BoardInterface::Tag::None)) result = t1;
            else if( (t4 == t5) && (t5 == t6) && (t4!=BoardInterface::Tag::None)) result = t4;
            else if( (t7 == t8) && (t8 == t9) && (t7!=BoardInterface::Tag::None)) result = t7;
            else if( (t1 == t4) && (t4 == t7) && (t1!=BoardInterface::Tag::None)) result = t1;
            else if( (t2 == t5) && (t5 == t8) && (t2!=BoardInterface::Tag::None)) result = t2;
            else if( (t3 == t6) && (t6 == t9) && (t3!=BoardInterface::Tag::None)) result = t3;
            else if( (t1 == t5) && (t5 == t9) && (t5!=BoardInterface::Tag::None)) result = t5;
            else if( (t3 == t5) && (t5 == t7) && (t5!=BoardInterface::Tag::None)) result = t5;
            else if(b.full()) result = BoardInterface::Tag::Tie;
            b.setWinTag(result);
    
                
            if(result == BoardInterface::Tag::O){
                putToGui("Player 1 make a line\n");
            }else if(result == BoardInterface::Tag::X){
                putToGui("Player 2 make a line\n");
            }else if(result == BoardInterface::Tag::Tie){
                putToGui("No line\n");
            }
            return;
        }
        bool checkGameover()
        {
            BoardInterface::Tag result = BoardInterface::Tag::None;
            BoardInterface::Tag t1, t2, t3, t4, t5, t6, t7, t8, t9;
            t1 = MainBoard.state(0, 0);
            t2 = MainBoard.state(0, 1);
            t3 = MainBoard.state(0, 2);
            t4 = MainBoard.state(1, 0);
            t5 = MainBoard.state(1, 1);
            t6 = MainBoard.state(1, 2);
            t7 = MainBoard.state(2, 0);
            t8 = MainBoard.state(2, 1);
            t9 = MainBoard.state(2, 2);
            if( (t1 == t2) && (t2 == t3) ) result = t1;
            else if( (t4 == t5) && (t5 == t6) ) result = t4;
            else if( (t7 == t8) && (t8 == t9) ) result = t7;
            else if( (t1 == t4) && (t4 == t7) ) result = t1;
            else if( (t2 == t5) && (t5 == t8) ) result = t2;
            else if( (t3 == t6) && (t6 == t9) ) result = t3;
            else if( (t1 == t5) && (t5 == t9) ) result = t5;
            else if( (t3 == t5) && (t5 == t7) ) result = t5;

            if(result == BoardInterface::Tag::O){
                std::cout << "Player 1 Win\n" << std::endl;
                m_P1 = nullptr;
                m_P2 = nullptr;
                last.first = -87;
                last.second = -87;
                win ++;
                MainBoard.reset();
                return true;
            }else if(result == BoardInterface::Tag::X){
                std::cout << "Player 2 Win\n" << std::endl;
                m_P1 = nullptr;
                m_P2 = nullptr;
                last.first = -87;
                last.second = -87;
                win_2 ++;
                MainBoard.reset();
                return true;
            }else{
                int num = 0;
                for(int i = 0 ; i < 3 ; i ++){
                    for(int j = 0 ; j < 3 ; j++){
                        if((MainBoard.sub(i,j)).full()) num++;
                    }
                }
                if(num == 9){
                    std::cout << "Tie\n";
                    m_P1 = nullptr;
                    m_P2 = nullptr;
                    last.first = -87;
                    last.second = -87;
                    win_3 ++;
                    MainBoard.reset();
                    return true;
                }
            }
            return false;
        }

        bool prepareState()
        {
            call(&AIInterface::init, m_P1, true);
            call(&AIInterface::init, m_P2, false);
            return true;
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            val.get();
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value == false, int> = 0 >
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            return val.get();
        }

        void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1+std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if( buf.back() == 0 ) buf.pop_back();
            gui->appendText( std::string(buf.begin(), buf.end()) );
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
        GUIInterface *gui;
        std::pair<int,int> last;
        UltraBoard MainBoard;
    };
}
