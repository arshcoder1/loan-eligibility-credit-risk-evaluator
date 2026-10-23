#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// Structure to store applicant information
struct Applicant {
    double annualIncome;
    double monthlyDebt;
    string repaymentHistory;
};

// Structure to store evaluation result
struct Evaluation {
    double dti;
    int score;
    string decision;
    string riskLevel;
};

// ------------------------------------------------------------
// INPUT VALIDATION
// ------------------------------------------------------------

bool isValidHistory(const string& history) {
    return history == "good" ||
           history == "average" ||
           history == "poor" ||
           history == "missing";
}

bool isValidApplicant(const Applicant& applicant) {

    if (applicant.annualIncome <= 0)
        return false;

    if (applicant.monthlyDebt < 0)
        return false;

    if (!isValidHistory(applicant.repaymentHistory))
        return false;

    return true;
}

// ------------------------------------------------------------
// DTI CALCULATION
// ------------------------------------------------------------

double calculateDTI(double annualIncome, double monthlyDebt) {

    double monthlyIncome = annualIncome / 12.0;

    if (monthlyIncome <= 0)
        return 100.0;

    return (monthlyDebt / monthlyIncome) * 100.0;
}

// ------------------------------------------------------------
// INCOME SCORE
// ------------------------------------------------------------

int calculateIncomeScore(double annualIncome) {

    if (annualIncome >= 600000)
        return 30;

    if (annualIncome >= 300000)
        return 20;

    return 10;
}

// ------------------------------------------------------------
// DTI SCORE
// ------------------------------------------------------------

int calculateDTIScore(double dti) {

    if (dti <= 20)
        return 35;

    if (dti <= 35)
        return 25;

    if (dti <= 50)
        return 15;

    return 0;
}

// ------------------------------------------------------------
// REPAYMENT HISTORY SCORE
// ------------------------------------------------------------

int calculateHistoryScore(const string& history) {

    if (history == "good")
        return 35;

    if (history == "average")
        return 20;

    if (history == "poor")
        return 5;

    // Missing repayment history
    return 0;
}

// ------------------------------------------------------------
// RISK LEVEL
// ------------------------------------------------------------

string getRiskLevel(int score) {

    if (score >= 75)
        return "Low Risk";

    if (score >= 50)
        return "Medium Risk";

    return "High Risk";
}

// ------------------------------------------------------------
// FINAL DECISION
// ------------------------------------------------------------

string getDecision(int score, double dti, const string& history) {

    // Very high debt automatically leads to rejection
    if (dti > 60)
        return "REJECT";

    // Missing repayment history requires manual review
    if (history == "missing")
        return "REVIEW";

    if (score >= 75)
        return "APPROVE";

    if (score >= 50)
        return "REVIEW";

    return "REJECT";
}

// ------------------------------------------------------------
// COMPLETE APPLICANT EVALUATION
// ------------------------------------------------------------

Evaluation evaluateApplicant(const Applicant& applicant) {

    Evaluation result;

    result.dti = calculateDTI(
        applicant.annualIncome,
        applicant.monthlyDebt
    );

    int incomeScore =
        calculateIncomeScore(applicant.annualIncome);

    int dtiScore =
        calculateDTIScore(result.dti);

    int historyScore =
        calculateHistoryScore(applicant.repaymentHistory);

    result.score =
        incomeScore +
        dtiScore +
        historyScore;

    result.riskLevel =
        getRiskLevel(result.score);

    result.decision =
        getDecision(
            result.score,
            result.dti,
            applicant.repaymentHistory
        );

    return result;
}

// ------------------------------------------------------------
// DISPLAY RESULT
// ------------------------------------------------------------

void displayResult(
    const Applicant& applicant,
    const Evaluation& result
) {

    cout << "\n========================================\n";
    cout << "        CREDIT RISK EVALUATION\n";
    cout << "========================================\n";

    cout << fixed << setprecision(2);

    cout << "Annual Income       : Rs. "
         << applicant.annualIncome << "\n";

    cout << "Monthly Debt        : Rs. "
         << applicant.monthlyDebt << "\n";

    cout << "Repayment History   : "
         << applicant.repaymentHistory << "\n";

    cout << "----------------------------------------\n";

    cout << "Debt-to-Income Ratio: "
         << result.dti << "%\n";

    cout << "Credit Risk Score   : "
         << result.score << "/100\n";

    cout << "Risk Level          : "
         << result.riskLevel << "\n";

    cout << "Final Decision      : "
         << result.decision << "\n";

    cout << "========================================\n";
}

// ------------------------------------------------------------
// 25 SELF-DESIGNED TEST CASES
// ------------------------------------------------------------

struct TestCase {

    Applicant applicant;
    string expectedDecision;
};

void runTestCases() {

    vector<TestCase> tests = {

        // Normal cases
        {{800000, 10000, "good"}, "APPROVE"},
        {{700000, 15000, "good"}, "APPROVE"},
        {{600000, 20000, "good"}, "APPROVE"},
        {{500000, 20000, "good"}, "APPROVE"},
        {{450000, 18000, "good"}, "APPROVE"},

        // Medium-risk cases
        {{500000, 30000, "average"}, "REVIEW"},
        {{400000, 25000, "average"}, "REVIEW"},
        {{300000, 20000, "average"}, "REVIEW"},
        {{350000, 15000, "average"}, "REVIEW"},
        {{250000, 12000, "average"}, "REVIEW"},

        // Poor repayment history
        {{800000, 10000, "poor"}, "REVIEW"},
        {{600000, 15000, "poor"}, "REVIEW"},
        {{400000, 10000, "poor"}, "REVIEW"},
        {{300000, 10000, "poor"}, "REJECT"},
        {{250000, 15000, "poor"}, "REJECT"},

        // High DTI
        {{800000, 45000, "good"}, "REVIEW"},
        {{600000, 40000, "good"}, "REVIEW"},
        {{500000, 50000, "good"}, "REJECT"},
        {{400000, 30000, "poor"}, "REJECT"},
        {{300000, 30000, "poor"}, "REJECT"},

        // Missing repayment history
        {{800000, 10000, "missing"}, "REVIEW"},
        {{500000, 15000, "missing"}, "REVIEW"},

        // Edge cases
        {{0, 10000, "good"}, "INVALID"},
        {{500000, -1000, "good"}, "INVALID"},
        {{500000, 10000, "unknown"}, "INVALID"}
    };

    int passed = 0;

    cout << "\n========================================\n";
    cout << "          RUNNING TEST CASES\n";
    cout << "========================================\n\n";

    for (int i = 0; i < tests.size(); i++) {

        const Applicant& applicant = tests[i].applicant;

        string actualDecision;

        if (!isValidApplicant(applicant)) {

            actualDecision = "INVALID";

        } else {

            Evaluation result =
                evaluateApplicant(applicant);

            actualDecision = result.decision;
        }

        if (actualDecision == tests[i].expectedDecision) {

            cout << "Test " << setw(2) << i + 1
                 << " : PASS\n";

            passed++;

        } else {

            cout << "Test " << setw(2) << i + 1
                 << " : FAIL"
                 << " | Expected: "
                 << tests[i].expectedDecision
                 << " | Got: "
                 << actualDecision << "\n";
        }
    }

    cout << "\n----------------------------------------\n";

    cout << "Tests Passed: "
         << passed << "/" << tests.size() << "\n";

    if (passed == tests.size()) {

        cout << "Result      : ALL TESTS PASSED\n";

    } else {

        cout << "Result      : SOME TESTS FAILED\n";
    }

    cout << "========================================\n";
}

// ------------------------------------------------------------
// MAIN PROGRAM
// ------------------------------------------------------------

int main() {

    cout << "========================================\n";
    cout << "  LOAN ELIGIBILITY & CREDIT RISK\n";
    cout << "          EVALUATOR\n";
    cout << "========================================\n";

    while (true) {

        cout << "\nChoose an option:\n";
        cout << "1. Evaluate a loan applicant\n";
        cout << "2. Run 25 test cases\n";
        cout << "3. Exit\n";

        cout << "\nEnter choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {

            Applicant applicant;

            cout << "\nEnter annual income (Rs.): ";
            cin >> applicant.annualIncome;

            cout << "Enter monthly debt (Rs.): ";
            cin >> applicant.monthlyDebt;

            cout << "Enter repayment history "
                 << "(good / average / poor / missing): ";

            cin >> applicant.repaymentHistory;

            // Validate input
            if (!isValidApplicant(applicant)) {

                cout << "\nInvalid applicant data.\n";

                if (applicant.annualIncome <= 0)
                    cout << "- Income must be greater than zero.\n";

                if (applicant.monthlyDebt < 0)
                    cout << "- Monthly debt cannot be negative.\n";

                if (!isValidHistory(applicant.repaymentHistory))
                    cout << "- Invalid repayment history.\n";

                continue;
            }

            Evaluation result =
                evaluateApplicant(applicant);

            displayResult(applicant, result);

        }

        else if (choice == 2) {

            runTestCases();

        }

        else if (choice == 3) {

            cout << "\nThank you for using the evaluator.\n";
            break;

        }

        else {

            cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}