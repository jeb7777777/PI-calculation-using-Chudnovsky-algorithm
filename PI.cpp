#include <gmpxx.h>
#include <fstream>
#include <string>

int main() {
    const unsigned long terms = 25;   // each term adds ~14 correct digits
    mpf_set_default_prec(1200);       // bits (~361 decimal digits)

    mpf_class sum = 0;
    for (unsigned long k = 0; k < terms; k++) {
        mpz_class f6k, f3k, fk, c;
        mpz_fac_ui(f6k.get_mpz_t(), 6 * k);                 // (6k)!
        mpz_fac_ui(f3k.get_mpz_t(), 3 * k);                 // (3k)!
        mpz_fac_ui(fk.get_mpz_t(), k);                      // k!
        mpz_ui_pow_ui(c.get_mpz_t(), 640320UL, 3 * k);      // 640320^(3k)

        mpz_class lin = mpz_class(545140134) * k;           // 545140134k + 13591409
        lin += 13591409;

        mpz_class num = f6k * lin;
        if (k % 2) num = -num;                              // (-1)^k
        mpz_class den = f3k * fk * fk * fk * c;             // (3k)! (k!)^3 640320^(3k)

        sum += mpf_class(num) / mpf_class(den);
    }

    // pi = 426880 * sqrt(10005) / sum
    mpf_class pi = mpf_class(426880) * sqrt(mpf_class(10005)) / sum;

    mp_exp_t exp;
    std::string digits = pi.get_str(exp, 10, 310);

    std::ofstream out("pi.txt");
    if (exp <= 0)
        out << "0." << std::string(-exp, '0') << digits << '\n';
    else
        out << digits.substr(0, exp) << '.' << digits.substr(exp) << '\n';
    return 0;
}