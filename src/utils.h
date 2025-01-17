#ifndef UTILS_H
#define UTILS_H

#define DIE(assertion, call_description)                        \
    do {                                                        \
        if (assertion) {                                        \
            fprintf(stderr, "(%s, %d): %s", __FILE__, __LINE__, \
                    call_description);                          \
            exit(EXIT_FAILURE);                                 \
        }                                                       \
    } while (0)

#endif /* UTILS_H */
