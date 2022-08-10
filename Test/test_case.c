void sort(int arr[], int len) {
  int i = 0;
  while (i < len - 1) {
    int j = i + 1;
    while (j < len) {
      if (arr[i] < arr[j]) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
      j = j + 1;
    }
    i = i + 1;
  }
}


int param32_arr(int a1[], int a2[], int a3[], int a4[], int a5[], int a6[],
                int a7[], int a8[], int a9[], int a10[], int a11[], int a12[],
                int a13[], int a14[], int a15[], int a16[], int a17[],
                int a18[], int a19[], int a20[], int a21[], int a22[],
                int a23[], int a24[], int a25[], int a26[], int a27[],
                int a28[], int a29[], int a30[], int a31[], int a32[]) {

  return 0;
}

int param16(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
            int a9, int a10, int a11, int a12, int a13, int a14, int a15,
            int a16) {
  int arr[16] = {a1, a2,  a3,  a4,  a5,  a6,  a7,  a8,
                 a9, a10, a11, a12, a13, a14, a15, a16};

  return 0;
}

int main() {
  int arr[32][2] = {{param16(getint(), getint(), getint(), getint(), getint(),
                             getint(), getint(), getint(), getint(), getint(),
                             getint(), getint(), getint(), getint(), getint(),
                             getint()),
                     8848}},
      i = 1;
  while (i < 32) {
    arr[i][0] = arr[i - 1][1] - 1;
    arr[i][1] = arr[i - 1][0] - 2;
    i = i + 1;
  }
  putint(param32_arr(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6],
                     arr[7], arr[8], arr[9], arr[10], arr[11], arr[12], arr[13],
                     arr[14], arr[15], arr[16], arr[17], arr[18], arr[19],
                     arr[20], arr[21], arr[22], arr[23], arr[24], arr[25],
                     arr[26], arr[27], arr[28], arr[29], arr[30], arr[31]));
  putch(10);
  return 0;
}
