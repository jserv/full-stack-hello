unsigned hash_djb2(const char *str, unsigned size)
{
    unsigned hash = size;
    unsigned c;

    while (c = *str) {
        hash = ((hash << 5) + hash) + c;
        ++str;
    }
    return hash % size;
}
