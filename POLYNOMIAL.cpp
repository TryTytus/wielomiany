// Adam Tyton

class POLYNOMIAL {
public:

    int* arr;
    int rank;
    static int overloaded;

    POLYNOMIAL() {
        arr = new int[1];
        arr[0] = 0;
        rank = 0;
    }

    POLYNOMIAL(int rank, ...) {
        arr = new int[rank + 1];
        this->rank = rank;

        va_list vaList;
        va_start(vaList, rank);

        for (int i = 0; i <= rank; i++) {
            arr[i] = va_arg(vaList, int);
        }

        toPrime();
    }


    POLYNOMIAL(const POLYNOMIAL& o) {
        rank = o.rank;
//        overloaded = o.overloaded;

        arr = new int[rank + 1];

        for (int i = 0; i <= rank; ++i) {
            arr[i] = o.arr[i];
        }
    }

    ~POLYNOMIAL() {
        delete[] arr;
    }



    POLYNOMIAL& operator= (POLYNOMIAL pol) {
        delete[] arr;
        rank = pol.rank;

        arr = new int[rank + 1];

        for (int i = 0; i <= rank; ++i) {
            arr[i] = pol.arr[i];
        }

        return *this;
    }

    POLYNOMIAL operator- () {
        POLYNOMIAL pol = POLYNOMIAL(*this);
        for (int i = 0; i <= rank; ++i) {
            pol.arr[i] = -pol.arr[i];
        }
        return pol;
    }

//    POLYNOMIAL& operator+= (POLYNOMIAL& pol) {
//
//    }

    int nwd(int a, int b)
    {
        while (a != 0)
        {
            int tmp = a;
            a = b % a;
            b = tmp;
        }
        return b;

        //return (a == 0) ? b : nwd(b % a, a);
    }

    void toPrime() {
        if (rank == 0) {
            if (arr[0] != 0)
                arr[0] = (arr[0] < 0) ? -1 : 1;
            return;
        }

        int div = arr[0];
        int j = 1;

        for (int i = 1; i <= rank; ++i) {
            div = nwd(div, arr[j++]);
        }

        if (div < 0)
            div = -div;

        for (int i = 0; i <= rank; ++i) {
            arr[i] /= div;
        }


    }

    void resize(int newRank) {
        int* arrNew = new int[newRank + 1];
        for (int i = 0; i <= rank; i++) {
            arrNew[i] = arr[i];
        }

        for (int i = rank + 1; i <= newRank; ++i) {
            arrNew[i] = 0;
        }

        delete[] arr;

        arr = arrNew;

    }

    void cutTo(int newRank) {
        int dif = rank - newRank;
        int* arrNew = new int[newRank + 1];
        for (int i = 0; i <= newRank; i++) {
            arrNew[i] = arr[i + dif];
            //arrNew[i] = arr[i - (rank - newRank)];
        }

        rank = newRank;
        delete[] arr;

        arr = arrNew;

    }

    int checkRank() {
        int newRank = rank;
        for (int i = rank; i >= 0 ; i--) {
            if (arr[i] == 0)
                newRank--;
            else break;
        }
        return newRank;
    }

    void multScalar(int scalar) {
        for (int i = 0; i <= rank; ++i) {
            arr[i] *= scalar;
        }
    }

    POLYNOMIAL operator+= (POLYNOMIAL& pol) {
        int newRank = rank;
        if (rank != pol.rank) {
            newRank = (rank > pol.rank) ? rank : pol.rank;
            resize(newRank);
            pol.resize(newRank);
        }

        for (int i = 0; i <= newRank; ++i) {
            arr[i] += pol.arr[i];
        }

        rank = newRank;

       
        int afterRank = checkRank();
        if (afterRank < newRank) {
            resize(afterRank);
        }

        rank = afterRank;
        
        toPrime();

        pol.resize(pol.rank);

        return *this;
    }

    POLYNOMIAL operator-= (POLYNOMIAL& pol) {
        pol = -pol;
        *this += pol;
        pol = -pol;
        return *this;
    }

    POLYNOMIAL operator+ (POLYNOMIAL pol) {
        POLYNOMIAL newPol = POLYNOMIAL(*this);
        newPol += pol;
        return newPol;
    }

    POLYNOMIAL operator- (POLYNOMIAL pol) {
        POLYNOMIAL newPol = POLYNOMIAL(*this);
        newPol -= pol;
        return newPol;
    }

    POLYNOMIAL operator<<= (int move) {
        if (move > (rank + 1)) return *this;
        cutTo(rank - move);
        return *this;
    }

    POLYNOMIAL operator>>= (int move) {

        // if (rank == 0 && arr[0] == 0)
        //     return *this;

        int newRank = rank + move;
        int* newArr = new int[newRank + 1];

        for (int i = 0; i < move; ++i) {
            newArr[i] = 0;
        }

        for (int i = move; i <= newRank; i++) {
            newArr[i] = arr[i - move];
        }

        delete[] arr;

        arr = newArr;
        rank = newRank;

        return *this;
    }

    POLYNOMIAL operator<< (int move) {
        POLYNOMIAL pol = POLYNOMIAL(*this);
        pol <<= move;
        return pol;
    }

    POLYNOMIAL operator>> (int move) {
        POLYNOMIAL pol = POLYNOMIAL(*this);
        pol >>= move;
        return pol;
    }


    POLYNOMIAL operator*= (POLYNOMIAL& pol) {

        int* newArr = new int[rank + pol.rank + 1];

        for (int i = 0; i <= rank; ++i) {
            for (int j = 0; j <= pol.rank; ++j) {
                newArr[i + j] = arr[i] * pol.arr[j];
            }
        }

        delete[] arr;
        arr = newArr;
        rank = rank + pol.rank;

        toPrime();

        return *this;
    }

    POLYNOMIAL operator/= (POLYNOMIAL pol) {
        if (rank < pol.rank) return *this;

        POLYNOMIAL cpPol = *this;

        int* newArr = new int[rank - pol.rank + 1];

        int j = rank - pol.rank;


        for (int i = rank; i >= pol.rank; i--) {
            pol >>= j;
            int div = cpPol.arr[i] /  pol.arr[i];
            newArr[j] = div;
            POLYNOMIAL p2 = pol;
            if (div != 0) {
                p2.multScalar(div);
                cpPol -= p2;
            }
            pol <<= j;
            j--;
        }

        delete[] arr;

        rank = rank - pol.rank;
        arr = newArr;


        return *this;
    }

    POLYNOMIAL operator%= (POLYNOMIAL& pol) {
        if (rank < pol.rank) return *this;

        POLYNOMIAL cpPol = *this;

        int j = rank - pol.rank;

        for (int i = rank; i >= pol.rank; i--) {
            pol >>= j;
            int div = cpPol.arr[i] /  pol.arr[i];
            POLYNOMIAL p2 = pol;
            if (div != 0) {
                p2.multScalar(div);
                cpPol -= p2;
            }
            pol <<= j;
            j--;
        }
        delete[] arr;

        rank = cpPol.checkRank();
        int* newArr = new int[rank + 1];

        for (int i = 0; i <= rank; ++i) {
            newArr[i] = cpPol.arr[i];
        }

        arr = newArr;

        return *this;
    }

    POLYNOMIAL operator* (POLYNOMIAL pol) {
        POLYNOMIAL cpPol = POLYNOMIAL(*this);
        cpPol *= pol;
        return cpPol;
    }

    POLYNOMIAL operator/ (POLYNOMIAL pol) {
        POLYNOMIAL cpPol = POLYNOMIAL(*this);
        cpPol /= pol;
        return cpPol;
    }

    POLYNOMIAL operator% (POLYNOMIAL pol) {
        POLYNOMIAL cpPol = POLYNOMIAL(*this);
        cpPol %= pol;
        return cpPol;
    }

    POLYNOMIAL& operator++ () {
        for (int i = 0; i <= rank; ++i) {
            arr[i] += 1;
        }
        return *this;
    }

    POLYNOMIAL operator++ (int) {
        POLYNOMIAL pol(*this);

        for (int i = 0; i <= rank; ++i) {
            pol.arr[i] += 1;
        }

        return pol;
    }

    POLYNOMIAL& operator-- () {
        for (int i = 0; i <= rank; ++i) {
            arr[i] -= 1;
        }
        return *this;
    }

    POLYNOMIAL operator-- (int) {
        POLYNOMIAL pol(*this);

        for (int i = 0; i <= rank; ++i) {
            pol.arr[i] -= 1;
        }

        return pol;
    }

    // void* operator new (size_t amount) {
    //     return ::new char[amount];
    // }

    // void operator delete (void* p) {
    //     ::delete (POLYNOMIAL*) p;
    // }


};

ostream& operator<< (ostream& stream, POLYNOMIAL pol) {
    stream << "( ";
    for (int i = 0; i < (pol.rank); i++) {
        stream << pol.arr[i] << ", ";
    }
    stream << pol.arr[pol.rank] << " )";
    return stream;
}

istream& operator>> (istream& stream, POLYNOMIAL& pol) {
    stream >> pol.rank;

    delete[] pol.arr;

    pol.arr = new int[pol.rank + 1];

    for (int i = 0; i <= pol.rank; ++i) {
        stream >> pol.arr[i];
    }
    return stream;
}

bool operator==(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    if (pol1.rank != pol2.rank)
        return false;

    bool isSame = true;
    for (int i = 0; i <= pol1.rank; ++i) {
        if (pol1.arr[i] != pol2.arr[i])
        {
            isSame = false;
            break;
        }
    }

    if (isSame)
        return true;

    POLYNOMIAL pol2N = -pol2;

    for (int i = 0; i <= pol2N.rank; ++i) {
        if (pol1.arr[i] != pol2N.arr[i])
            return false;
    }
    return true;
}

bool operator>(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    if (pol1.rank > pol2.rank)
        return true;
    if (pol1.rank < pol2.rank)
        return false;

    for (int i = pol1.rank; i > -1; --i) {
        if (pol1.arr[i] > pol2.arr[i])
            return true;
        if (pol1.arr[i] < pol2.arr[i])
            return false;
    }

    return false;
}

bool operator<(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    if (pol1.rank < pol2.rank)
        return true;
    if (pol1.rank > pol2.rank)
        return false;

    for (int i = pol1.rank; i > -1; --i) {
        if (pol1.arr[i] < pol2.arr[i])
            return true;
        else if (pol1.arr[i] < pol2.arr[i])
            return false;
    }

    return false;
}

bool operator<=(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    return !(pol1 > pol2);
}

bool operator>=(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    return !(pol1 < pol2);
}

bool operator!=(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    return !(pol1 == pol2);
}
