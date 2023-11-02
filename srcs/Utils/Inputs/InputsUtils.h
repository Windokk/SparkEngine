#include"../../Libs/ImGui_Lib/imgui.h"

enum class AlphaNumericKeys {
    // Alphabets
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    // Numbers
    Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
    // Special Characters
    Space, Exclamation, DoubleQuote, Hash, Dollar, Percent, Ampersand,
    SingleQuote, LeftParenthesis, RightParenthesis, Asterisk, Plus, Comma,
    Minus, Period, ForwardSlash, Colon, Semicolon, LessThan, Equals,
    GreaterThan, Question, At, LeftSquareBracket, Backslash, RightSquareBracket,
    Caret, Underscore, Backtick, LeftCurlyBrace, VerticalBar, RightCurlyBrace,
    Tilde,
    // Gamepad Keys
    ButtonA, ButtonB, ButtonX, ButtonY, LeftBumper, RightBumper,
    LeftTrigger, RightTrigger, Back, Start, LeftStick, RightStick,
    DPadUp, DPadDown, DPadLeft, DPadRight
};
