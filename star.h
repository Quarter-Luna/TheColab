#ifndef __TAR_H__
#define __TAR_H__
#include <sys/stat.h>
#define HELP_MAX 100000
#define TRUE 1
#define FALSE 0
#define MAGIC "ustar"
#define SUCCESS 0
#define ERR -1

/**
 * header_ustar structure
 */
struct header_ustar
{
    /**
     * Pathname, stored as a null-terminated string except when all characters in
     * the array contain non-NUL characters including the last character.One common
     * early convention used a trailing "/"character to indicate a directory name,
     */
    char name[100];
    //! File mode, stored as an octal number in ASCII.
    char mode[8];
    //! User id of owner, as octal numbers in ASCII.
    char uid[8];
    //! Group id of owner, as octal numbers in ASCII.
    char gid[8];
    /**
     * Size of file, as octal number in ASCII. For regular files only,
     * this indicates the amount of data that follows the header. In
     * particular,zero is stored for anything other than regular files.
     */
    char size[12];
    /**
     * Modification time of file, as an octal number in ASCII. This
     * indicates the number of seconds since the start of the epoch,
     * 00:00:00 UTC January 1, 1970.
     */
    char mtime[12];
    /**
     * Header checksum, stored as an octal number in ASCII. To compute the checksum,
     * set the checksum field to all spaces, then sum all bytes in the header using
     * unsigned arithmetic. This field should be stored as six octal digits followed
     * by a null and a space character.
     */
    char checksum[8];
    /**
     * Type of entry. POSIX extended the earlier linkflag field with several new
     * type values:
     *     * '0'   Regular file.
     *     * '1'   Hard link.
     *     * '2'   Symbolic link.
     *     * '3'   Character device node.
     *     * '4'   Block device node.
     *     * '5'   Directory.
     *     * '6'   FIFO node.
     *     *  Other treated as a regular file.
     * Note that in this tar implementation only Regular files and Directories are implemented.
     */
    char typeflag;
    /**
     *Not used in my implementation
     */
    char linkname[100];
    /**
     * Contains the magic value "ustar" followed by a NUL byte to
     * indicate that this is a POSIX standard archive. Full compliance
     * requires the uname and gname fields be properly set.
     */
    char magic[6];
    /**
     * Version. This should be "00" (two copies of the ASCII digit zero) for
     * POSIX standard archives without the NUL byte.
     */
    char version[2];
    /**
     * Not used in my implementation. The uid is used instead.
     */
    char uname[32];
    /**
     * Not used in my implementation. The gid is used instead.
     */
    char gname[32];
    /**
     * Not used in my implementation.
     */
    char devmajor[8];
    /**
     * Not used in my implementation.
     */
    char devminor[8];
    /**
     * If the pathname is too long to fit in the 100 bytes provided by name , it can
     * be split at any / character with the first portion going into the prefix
     * field. If the prefix field is not empty, the path name will be in form
     * of `prefix/name`
     */
    char prefix[155];
    /**
     * ustar tape archive is used to store data on 9-track industry-standard 12.7 mm (0.5 in).
     * Data is read and written in form of consecutive logical record each of size 512 bytes.
     * padding is used to make the header logical record fit on 512 byte border.
     */
    char padding[12];
} __attribute__((packed));

/**
 * \breif reads the file from tar archive and write it
 */
void regular_untar(FILE *tarf, struct header_ustar *h);
/*!
 * \brief writes the tar header to the file content to the tar file
 * \param tarf pointer to the tar file
 * \param filename pathname of the file
 * \param filestat pointer to the structure stat that contains file state info
 * the function regular_tar creates struct header_ustar and fill it with info
 * from the filestat then write this data to the file pointer to by tarf
 * next the content of the file is written all in binary..
 * after the content of file is written the stream tarf is filled with zeros
 * till it is alligned on boundry of 512 bytes
 */
void regular_tar(FILE *tarf, char *filename, struct stat *filestat);
/*!
 * \brief creates directory with the name carried by the header_ustar *h
 * \param h pointer to the ustar_header that contains directory info
 * the dir dir_untar function creates directory with name found in prefix/name
 * members of the struct header_ustar. No persmissions are set on created
 * folder
 */

void dir_untar(struct header_ustar *h);
/*!
 * \brief add directory and its content to the tar archive
 * \param tarf pointer to the tar file structure
 * \param dirname null terminated directory name
 * \param s pointer to the directory state structure
 * the function dir_tar will create struct header_ustar for the directory
 * then write it to the tarf steam then iterate over all the direcotry content
 * and tar them either with dir_tar or with regular_tar accordint to the
 * directory type.
 */
void dir_tar(FILE *tarf, char *dirname, struct stat *s);
/*!
 * read HELP_MAX character from file called help.doc
 * and print write them to stdout
 */


unsigned int checksum(const struct header_ustar *h);

/*!
 * \brief create pack list of files into one tar archive
 * \param c number of elementes in the array of strings names
 * \param names the first element in names represent the name of the tar
 * and the rest (c-1) names represent the files/folders to be tarred
 *
 * tar creates file with Pathnam = names[0] and it decides the type of the
 * given names and call either regular_tar or dir_tar to do both regular file
 * tarring and directory tarring
 */
void tar(int c, char **names);

/*!
 * \brief unpack the tar archive into its original contents
 *
 * \param name the Pathname of the tar archive
 *
 * untar reads header_ustar logical block from the file and use it to determine the type
 * of the file. and then passes this block to either regular_untar or dir_untar based on
 * the member type_flag
 */
void untar(char *name);

/*!
 * \brief Function thar returns TRUE if the given file path is correct
 * and FALSE otherwise.
 *
 * \param path Pathname of the file.
 *
 * file_exist tries opening file with fopen and returns TRUE if fopen succeeded
 * and FALSE if fopen failed
 */
int file_exist(char *path);

/*!
 * \brief Function that returns 1 if the given struct is
 * valid header_ustar and zero otherwise
 *
 * \param h pointer to the struct that will be checked
 *
 * this function makesure that the h->magic is identical to the MAGIC
 * number and that the h->checksum of the struct is the right
 * checksum
 */
int isvalid_tar(const struct header_ustar *h);
#endif