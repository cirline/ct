
class Ventor {
    public:
    Ventor(int n);

    double & operator[](int i);

    int size();

    private:
    double * elem;
    int len;
};

