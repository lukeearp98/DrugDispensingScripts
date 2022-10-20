#include <stdio.h>

int main(void) {
    FILE *fp = fopen("testCSV.csv", "r");

    if (!fp) {
        printf("Can't open file\n");
        return 0;
    }

    char buf[1024];
    int row_count = 0;
    int field_count = 0;
    while (fgets(buf, 1024, fp)) {
        field_count = 0;
        row_count++;

        if (row_count == 1) {
            continue;
        }

        char *field = strtok(buf, ",");
        while (field) {
            if (field_count == 0) {
                printf("Date: ");
            }
            if (field_count == 1) {
                printf("Time: ");
            }
            if (field_count == 2) {
                printf("Controller Name: ");
            }
            if (field_count == 3) {
                printf("Patient ID: ");
            }
            if (field_count == 4) {
                printf("Drug Name: ");
            }
            if (field_count == 5) {
                printf("Dosage Amount: ");
            }

            printf("%s\n", field);
            field = strtok(NULL, ",");

            field_count++;
        }
        printf("\n");
    }

    fclose(fp);

    return 0;
}
