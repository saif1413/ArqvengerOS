#include "drivers/ext2/internal.h"

int read_sectors(struct ext2* fs, unsigned long long sector, size_t sectors, void* buffer) {
    return ata_read(fs->firstSector + sector, sectors, buffer);
}

int read_block(struct ext2* fs, size_t block) {

    if (block > fs->sb->totalBlocks) {
        return -1;
    }

    return read_sectors(fs, block * fs->sectorsPerBlock, fs->sectorsPerBlock, fs->blockBuffer);
}

int read_block_fragment(struct ext2* fs, size_t block, size_t offset, size_t len, void* buffer) {

    if (read_block(fs, block) == -1) {
        return -1;
    }

    char* dest = buffer;
    char* origin = (char*) fs->blockBuffer + offset;

    for (size_t i = 0; i < len; i++) {
        dest[i] = origin[i];
    }

    return 0;
}
