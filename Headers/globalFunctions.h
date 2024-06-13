bool IsEven (double value) {
    return static_cast<int>(value) % 2 == 0;
}

bool IsInt (double value) {
    return value == static_cast<int>(value);
}