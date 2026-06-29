#pragma once

#include "../../../devices/IPointer.hpp"

#include "gestures/ITrackpadGesture.hpp"
#include "GestureTypes.hpp"

#include <vector>
#include <expected>
#include <optional>

class CTrackpadGestures {
  public:
    void                             clearGestures();
    std::expected<void, std::string> addGesture(UP<ITrackpadGesture>&& gesture, size_t fingerCount, eTrackpadGestureDirection direction, uint32_t modMask, float deltaScale,
                                                bool disableInhibit, std::optional<uint32_t> button);
    std::expected<void, std::string> removeGesture(size_t fingerCount, eTrackpadGestureDirection direction, uint32_t modMask, float deltaScale, bool disableInhibit,
                                                   std::optional<uint32_t> button);

    void                             gestureBegin(const IPointer::SSwipeBeginEvent& e);
    void                             gestureUpdate(const IPointer::SSwipeUpdateEvent& e);
    void                             gestureEnd(const IPointer::SSwipeEndEvent& e);

    void                             gestureBegin(const IPointer::SPinchBeginEvent& e);
    void                             gestureUpdate(const IPointer::SPinchUpdateEvent& e);
    void                             gestureEnd(const IPointer::SPinchEndEvent& e);

    void                             gestureButtonPressed(const IPointer::SButtonEvent& e);
    void                             gestureButtonReleased(const IPointer::SButtonEvent& e);
    void                             gestureMotion(const IPointer::SMotionEvent& e);

    eTrackpadGestureDirection        dirForString(const std::string_view& s);

  private:
    void        resetMouseGestureState();
    const char* stringForDir(eTrackpadGestureDirection dir);
    struct SGestureData {
        UP<ITrackpadGesture>      gesture;
        size_t                    fingerCount      = 0;
        uint32_t                  modMask          = 0;
        eTrackpadGestureDirection direction        = TRACKPAD_GESTURE_DIR_NONE; // configured dir
        float                     deltaScale       = 1.F;
        bool                      disableInhibit   = false;
        std::optional<uint32_t>   button           = std::nullopt;
        eTrackpadGestureDirection currentDirection = TRACKPAD_GESTURE_DIR_NONE; // actual dir of that select swipe
    };

    std::vector<SP<SGestureData>> m_gestures;

    Vector2D                      m_currentTotalDelta      = {};
    SP<SGestureData>              m_activeGesture          = nullptr;
    bool                          m_gestureFindFailed      = false;
    bool                          m_activeGestureFromMouse = false;
    std::optional<uint32_t>       m_pendingMouseGestureButton;
    std::optional<uint32_t>       m_activeMouseGestureButton;
};

inline UP<CTrackpadGestures> g_pTrackpadGestures = makeUnique<CTrackpadGestures>();
