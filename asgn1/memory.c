#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main() {

    char command[4];
    if (read(0, command, 4) >= 4) { //read the first 4
        if (strcmp(command, "get ") == 0) {
            char file[4096];
            char buf[1];
            int bytes_read;
            int index = 0;
            int fd = -1;

            int nAppear = 0;

            do {
                bytes_read = read(0, buf, 1);
                if (bytes_read > 0) {

                    if (buf[0] == '\n') {
                        file[index] = '\0';
                        // puts(file);
                        nAppear = 1;

                        fd = open(file, O_RDONLY, 0777);
                        break;

                    } else if (nAppear == 0 && (buf[0] == '\0' || buf[0] == ' ')) {
                        write(2, "Invalid Command\n", 16);
                        return 1;
                    } else if (buf[0] >= 0 && buf[0] <= 255) {
                        file[index] = buf[0];
                        index++;
                    } else {
                        write(2, "Invalid Command\n", 16);
                        return 1;
                    }
                }
            } while (bytes_read > 0);

            if (fd < 0) {
                //tried get but the filename not exist in the directory
                write(2, "Invalid Command\n", 16);
                return 1;
            } else {
                int read_more;
                char inner_buf[4096];
                do {
                    read_more = read(fd, inner_buf, 4096);
                    if (read_more > 0) {
                        write(1, inner_buf, read_more);
                    }
                } while (read_more > 0);
                close(fd);
            }

        } else if (strcmp(command, "set ") == 0) {

            char file[4096];
            char buf[1];
            int bytes_read;
            int index = 0;
            int fd;

            int nAppear = 0;

            do {
                bytes_read = read(0, buf, 1);
                if (bytes_read > 0) {

                    if (buf[0] == '\n') {
                        file[index] = '\0';
                        // puts(file);
                        nAppear = 1;

                        fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0777);

                        int read_more;
                        char inner_buf[4096];
                        do {
                            read_more = read(0, inner_buf, 4096);
                            if (read_more > 0) {
                                write(fd, inner_buf, read_more);
                            }
                        } while (read_more > 0);

                        close(fd);

                    } else if (nAppear == 0 && (buf[0] == '\0' || buf[0] == ' ')) {
                        write(2, "Invalid Command\n", 16);
                        return 1;
                    } else if (buf[0] >= 0 && buf[0] <= 255) {
                        file[index] = buf[0];
                        index++;
                    } else {
                        write(2, "Invalid Command\n", 16);
                        return 1;
                    }
                }
            } while (bytes_read > 0);

        } else {
            //but reads other remaining buffer as well...
            write(2, "Invalid Command\n", 16);
            return 1;
        }

    } else {
        write(2, "Invalid Command\n", 16);
        return 1;
    }
}
