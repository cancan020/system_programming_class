#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int txtfile(const char *filename) {
    int len = strlen(filename);
 if (len > 4 && strcmp(filename + len - 4, ".txt") == 0) {
        return 1; }
    return 0;
}

int main(int argc, char *argv[]) {

   if (argc != 3) {
      printf("사용법: %s <원본_디렉터리> <결과_파일>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *source_directory_path = argv[1];
    char *output_file_path = argv[2];

    DIR *dir_stream;
    struct dirent *dir_entry;
    int output_fd;


    if ((dir_stream = opendir(source_directory_path)) == NULL) {
        printf("원본 디렉터리를 열 수 없습니다");
        exit(EXIT_FAILURE);}

   output_fd = open(output_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        printf("결과 파일을 열 수 없습니다");
        closedir(dir_stream);
        exit(EXIT_FAILURE); }

    printf("'%s' 디렉터리에서 .txt 파일 병합을 시작합니다\n", source_directory_path);


    while ((dir_entry = readdir(dir_stream)) != NULL) {

        if (txtfile(dir_entry->d_name)) {

            char source_file_path[1024];

            snprintf(source_file_path, sizeof(source_file_path), "%s/%s", source_directory_path, dir_entry->d_name);

            printf("처리 파일: %s\n", source_file_path);


            int input_fd = open(source_file_path, O_RDONLY);
            if (input_fd == -1) {
                printf("파일을 열 수 없어 건너뜁니다");
                continue;
            }

            char buffer;
            ssize_t bytes_read;


            while ((bytes_read = read(input_fd, &buffer, 1)) > 0) {

                if (buffer != ' ') {
                    if (write(output_fd, &buffer, 1) == -1) {
                        printf("결과 파일에 쓰는 중 문제가 발생했습니다");
                        close(input_fd);
                        close(output_fd);
                        closedir(dir_stream);
                        exit(EXIT_FAILURE);
                    }
                }
            }

            if (bytes_read == -1) {
                printf("파일을 읽는 중 문제가 발생했습니다");
            }

           close(input_fd);
   }
}

    printf("결과가 '%s' 파일에 저장되었습니다.\n", output_file_path);

   closedir(dir_stream);
   close(output_fd);

    return 0;
}
