

// SPI_main.c
// Eenvoudig SPI-main voorbeeld voor Raspberry Pi met spidev
// Praat met een STM32 die als SPI-sub draait.

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

static int spi_open(const char *device, uint32_t mode, uint32_t speed_hz, uint8_t bits_per_word) {
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open(spidev)");
        return -1;
    }

    // Mode (bijv. SPI_MODE_0)
    if (ioctl(fd, SPI_IOC_WR_MODE32, &mode) == -1) {
        perror("ioctl(SPI_IOC_WR_MODE32)");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_RD_MODE32, &mode) == -1) {
        perror("ioctl(SPI_IOC_RD_MODE32)");
        close(fd);
        return -1;
    }

    // Bits per word (meestal 8)
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        perror("ioctl(SPI_IOC_WR_BITS_PER_WORD)");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits_per_word) == -1) {
        perror("ioctl(SPI_IOC_RD_BITS_PER_WORD)");
        close(fd);
        return -1;
    }

    // Max speed (Hz)
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz) == -1) {
        perror("ioctl(SPI_IOC_WR_MAX_SPEED_HZ)");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed_hz) == -1) {
        perror("ioctl(SPI_IOC_RD_MAX_SPEED_HZ)");
        close(fd);
        return -1;
    }

    return fd;
}



// Full-duplex transfer: tx data versturen + rx data ontvangen in één transactie
static int spi_transfer(int fd, const uint8_t *tx, uint8_t *rx, size_t len, uint32_t speed_hz) {
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = (uint32_t)len,
        .speed_hz = speed_hz,     // je kunt per transfer een (lagere) speed kiezen
        .delay_usecs = 0,
        .bits_per_word = 8,
        .cs_change = 0            // 0 = CS gaat hoog na de transfer; 1 = CS blijft laag
    };

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        perror("ioctl(SPI_IOC_MESSAGE)");
        return -1;
    }
    return 0;
}


int main(void) {
    const char *dev_sub1 = "/dev/spidev0.0"; // CE0 → STM32 #1
    uint32_t mode = SPI_MODE_0;                // CPOL=0, CPHA=0
    uint8_t bits = 8;                          // 8 bits per woord
    uint32_t speed = 250000;                   // 250 kHz (veilig starten)

    // 1) Open SPI device
    int fd_sub1 = spi_open(dev_sub1, mode, speed, bits);
    if (fd_sub1 < 0) {
        fprintf(stderr, "Kon SPI niet openen\n");
        return 1;
    }

    // 2) Voorbeeld: stuur 1 byte en lees 1 byte terug (full duplex)
    uint8_t tx[1] = { 0x55 };  // voorbeeld commando
    uint8_t rx[1] = { 0x00 };

    if (spi_transfer(fd_sub1, tx, rx, sizeof(tx), speed) != 0) {
        fprintf(stderr, "SPI transfer mislukt\n");
        close(fd_sub1);
        return 1;
    }

    printf("Verstuurd: 0x%02X, Ontvangen: 0x%02X\n", tx[0], rx[0]);
    // Verwacht dat de STM32 iets terugstuurt (bijv. een vaste byte), zodat je weet dat het werkt.

    // 3) Sluit SPI
    close(fd_sub1);



    const char *dev_sub2 = "/dev/spidev0.1"; // CE1, sub2

    // 1) Open SPI device
    int fd_sub2 = spi_open(dev_sub2, mode, speed, bits);
    if (fd_sub2 < 0) {
        fprintf(stderr, "Kon SPI niet openen\n");
        return 1;
    }

    // 2) Voorbeeld: stuur 1 byte en lees 1 byte terug (full duplex)
    uint8_t tx2[1] = { 0x55 };  // voorbeeld commando
    uint8_t rx2[1] = { 0x00 };

    if (spi_transfer(fd_sub2, tx2, rx2, sizeof(tx2), speed) != 0) {
        fprintf(stderr, "SPI transfer mislukt\n");
        close(fd_sub2);
        return 1;
    }

    printf("Verstuurd: 0x%02X, Ontvangen: 0x%02X\n", tx2[0], rx2[0]);
    // Verwacht dat de STM32 iets terugstuurt (bijv. een vaste byte), zodat je weet dat het werkt.

    // 3) Sluit SPI
    close(fd_sub2);
    return 0;
}

