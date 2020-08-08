#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <functional>
#include <unordered_map>
#include <vector>

class InputManager
{
public:
    using KeyCallback = std::function<void(int, int)>;
    using MouseMoveCallback = std::function<void(float xpos, float ypos)>;

    static void addKeyBinding(int key, const KeyCallback& callback)  
    {
        m_keyCallbacks[key].push_back(callback);
    }

    static void onKeyPress(int key, int action)
    {
        for (KeyCallback& callback : m_keyCallbacks[key])
        {
            callback(key, action);
        }
    }

    static void addMouseMoveCallback(const MouseMoveCallback& callback)
    {
        m_mouseMoveCallbacks.push_back(callback);
    }

    static void onMouseMove(float xpos, float ypos)
    {
        for (MouseMoveCallback& callback : m_mouseMoveCallbacks)
        {
            callback(xpos, ypos);
        }
    }

private:

    static std::unordered_map<int, std::vector<KeyCallback>> m_keyCallbacks;
    static std::vector<MouseMoveCallback> m_mouseMoveCallbacks;
};

#endif //INPUTMANAGER_HPP
