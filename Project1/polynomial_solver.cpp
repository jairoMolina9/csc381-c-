#include "polynomial_solver.h"

/*Constructor*/
PolynomialSolver::PolynomialSolver(std::string file_name) : file_operator(file_name) {}

/*Saves canonical and regular form in separate vectors and execute the required arithmetic commands*/
void PolynomialSolver::Execute() {
    //stores all regular form polynomials
    std::string s_1 = file_operator.ReadRegular();
    //holds all canonical form polynomials
    std::string s = file_operator.ReadCanonical(); 
    std::string delimeter = "//";
    size_t pos = 0;
    std::string poly;

    //vector of all canonical form polynomials
    std::vector<std::string> cano_v;
    //vector of all regular form polynomials
    std::vector<std::string> reg_v;

    //populates all canonical form polynomials in vector cano_v
    while ((pos = s.find(delimeter)) != std::string::npos) {
        poly = s.substr(0, pos);
        //removes empty space at end of canonical form added by linked list
        poly.pop_back();
        cano_v.push_back(poly);
       s.erase(0, pos + delimeter.length());
    }

    //populates all regular form polynomials in vector reg_v
    while ((pos = s_1.find(delimeter)) != std::string::npos) {
        poly = s_1.substr(0, pos);
        reg_v.push_back(poly);
        s_1.erase(0, pos + delimeter.length());
    }

    int pair = 1;
    std::string cano_A, cano_B, reg_A, reg_B;

    for (size_t i = 0; i < cano_v.size(); i++) {
        if (pair == 1) {
            cano_A = cano_v[i];
            reg_A = reg_v[i];
            pair++;
        }
        else if (pair == 2) {
            cano_B = cano_v[i];
            reg_B = reg_v[i];
            /*Write as they appear in file*/
            file_operator.Write("Regular A: "+reg_A+"\n");
            file_operator.Write("Regular B: "+reg_B+"\n");

            /*Write in canonical form*/
            file_operator.Write("Canonical A: "+cano_A+"\n");
            file_operator.Write("Canonical B: " + cano_B+"\n");

            /*Write their sum*/
            file_operator.Write("sum: "+Sum(cano_A, cano_B)+"\n");
            /*Write their difference*/
            file_operator.Write("diff: " + Subtract(cano_A, cano_B) + "\n");
            /*Write their product*/
            file_operator.Write("multiply: " + Multiply(cano_A, cano_B) + "\n\n");
            pair = 1;
            cano_A = cano_B = reg_A = reg_B = "";
        }
    }
}

std::string PolynomialSolver::Sum(std::string poly_A, std::string poly_B)
{
    poly_A.append(" ");
    poly_A.append(poly_B);

    return file_operator.CanonicalForm(poly_A, 1);
}

std::string PolynomialSolver::Subtract(std::string poly_A, std::string poly_B)
{
    poly_A.append(" ");
    poly_B = file_operator.CanonicalForm(poly_B, 2); //canonical form and multiply by -1
    poly_A.append(poly_B);
    return file_operator.CanonicalForm(poly_A, 1);
}

std::string PolynomialSolver::Multiply(std::string poly_A, std::string poly_B)
{
    return file_operator.CanonicalFormMultiply(poly_A, poly_B);
}
