#include <iostream>
#define bitlength 30

class customint{
public:
    unsigned int val : bitlength;
    customint(unsigned int val){
        this->val = val;
    }
    
    customint& operator =(unsigned int x){
        this->val = x;
        return *this;
    }

    customint& operator+(const customint& x){
        this->val += x.val;
        return *this;
    }

    customint& operator*(const customint& x){
        this->val *= x.val;
        return *this;
    }

    customint& operator/(const int x){
        this->val /= x;
        return *this;
    }

    customint& operator <<(unsigned int x){
        std::cout << "<<\n";
        this->val <<= x;
        return *this;
    }

    customint& operator >>(unsigned int x){
        std::cout << ">>\n";
        this->val >>= x;
        return *this;
    }

    unsigned int operator%(unsigned int x){
        return this->val % x;
    }
};

std::ostream& operator<<(std::ostream& os, const customint& x)
{
    os << x.val;
    return os;
}

bool operator==(const customint &a, const customint &b){
    // std::cout << "==\n";
    return a.val == b.val;
}

bool operator!=(const customint &a, const customint &b){
    // std::cout << "!=\n";
    return a.val != b.val;
}

bool operator<(const customint &a, const customint &b){
    // std::cout << "<\n";
    return a.val < b.val;
}

bool operator<(const int a, const customint &b){
    // std::cout << "<\n";
    return a < b.val;
}

bool operator<=(const customint &a, const customint &b){
    // std::cout << "<=\n";
    return a.val <= b.val;
}

bool operator>(const customint &a, const customint &b){
    // std::cout << ">\n";
    return a.val > b.val;
}

bool operator>=(const customint &a, const customint &b){
    // std::cout << ">=\n";
    return a.val >= b.val;
}

int operator-(const customint &a, int b){
    return a.val - b;
}

int operator-(const int b, const customint &a){
    return b - a.val;
}

int operator*(const int a, const customint &b){
    return a*b.val;
}

int operator|(const int a, const customint &b){
    return a | b.val;
}

int operator|(const customint &a, const customint &b){
    return a.val | b.val;
}



// customint& operator-(const customint &a, int b){
    // customint temp = new customint(a.val - b);
    // return temp;
// }

// int operator<<(const customint &a, const customint &b){
//     std::cout << "<<\n";
//     return a.val << b.val;
// }