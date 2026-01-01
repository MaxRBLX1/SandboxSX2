#pragma once
#include <string>
#include <vector>

// Push a new debug message into the bus
void dbgPush(const std::string& msg);

// Get the latest debug message
const std::string& dbgLast();

// Clear all messages
void dbgClear();

// Dump all messages as a vector
std::vector<std::string> dbgDump();