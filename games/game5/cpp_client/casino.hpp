#ifndef _CASINO_HPP_
#define _CASINO_HPP_


class casino{
private:
    static int slots;
public:

    static int init_func(){
        return 1;
    }

    static int my_func(){
        return 0;
    }
};


int casino::slots;

#endif