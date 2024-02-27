#include <stdio.h>

int main() {
    int frames;
    printf("Enter the number of frames: ");
    scanf("%d", &frames);

    int incomingStream[100];
    int numPages;
    printf("Enter the number of pages in the incoming stream (max 100): ");
    scanf("%d", &numPages);

    printf("Enter the page reference stream:\n");
    for (int i = 0; i < numPages; i++) {
        scanf("%d", &incomingStream[i]);
    }

    int pageFaults = 0;
    int temp[frames];

    for (int m = 0; m < frames; m++) {
        temp[m] = -1;
    }

    printf("\nIncoming\t\tFrame 1\t\t\tFrame 2\t\t\tFrame 3\n");

    for (int m = 0; m < numPages; m++) {
        int s = 0;

        for (int n = 0; n < frames; n++) {
            if (incomingStream[m] == temp[n]) {
                s++;
                pageFaults--;
            }
        }

        pageFaults++;

        if ((pageFaults <= frames) && (s == 0)) {
            temp[m] = incomingStream[m];
        } else if (s == 0) {
            temp[(pageFaults - 1) % frames] = incomingStream[m];
        }

        printf("\n%d\t\t\t", incomingStream[m]);

        for (int n = 0; n < frames; n++) {
            if (temp[n] != -1)
                printf("%d\t\t\t", temp[n]);
            else
                printf("-\t\t\t");
        }
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);

    return 0;
}
