
/**
 * @file FinanceManager.h
 * @author TV1 (Vy)
 * @brief Declares the main application controller.
 */

#pragma once

class FinanceManager {
public:
    void run();

private:
    void displayMenu() const;
    bool handleChoice(int choice) const;
};
