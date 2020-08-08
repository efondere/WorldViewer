#include "InputManager.hpp"

std::unordered_map<int, std::vector<InputManager::KeyCallback>> InputManager::m_keyCallbacks;
std::vector<InputManager::MouseMoveCallback> InputManager::m_mouseMoveCallbacks;
