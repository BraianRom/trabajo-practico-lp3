double reciprocal(int i) {
    if (i == 0) {
        printf("Error: division por cero\n");
        return 0;
    }
    return 1.0 / i;
}
#ifdef __cplusplus
extern "C" {
#endif

extern double reciprocal (int i);

#ifdef __cplusplus
}
#endif
