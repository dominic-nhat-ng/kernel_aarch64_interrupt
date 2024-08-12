#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/read_module" // Đường dẫn đến device file

int main() {
    int fd;
    unsigned long daif_value;
    char buffer[32];

    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device file");
        return -1;
    }

    if (read(fd, buffer, sizeof(buffer)) < 0) {
        perror("Failed to read DAIF value");
        close(fd);
        return -1;
    }

    if (sscanf(buffer, "0x%lx", &daif_value) != 1) { // Đọc giá trị hex
        perror("Failed to parse DAIF value");
        close(fd);
        return -1;
    }

    close(fd);

    printf("DAIF register value: 0x%lx\n", daif_value);

    return 0;
}

