#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <vector>

enum Mode { USER_MODE, MONITOR_MODE };

struct InterruptVector {
    std::vector<int> positions;
    std::vector<void (*)()> routines;
};

void delayRoutine();
void monitorMode(InterruptVector &iv, int interruptNumber);
void userMode(const std::string &filename);

#endif