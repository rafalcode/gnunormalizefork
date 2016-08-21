/* 
 * inspiration from http://yannesposito.com/Scratch/en/blog/2010-10-14-Fun-with-wav/ 
 *
 * can be basis of a canonical wav checker */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define READBLKSZ 512

#define ARBSZ 128

typedef struct
{
    char id[4]; // should always contain "RIFF"
    int glen;    // general length: total file length minus 8, could because the types so far seen (id and glen itself) are actually 8 bytes
    char fstr[8]; // format string should be "WAVEfmt ": actually two 4chartypes here.
    int fmtnum; // format number 16 for PCM format
    short pcmnum; // PCM number 1 for PCM format
    short nchans; // num channels
    int sampfq; // sampling frequency: CD quality is 44100, 48000 is also common.
    int bps; // bytes_per_second (aka, BlockAlign) = numchannels* bytes per sample*samplerate. stereo shorts at 44100 . should be 176k.
    short bbc; // bytes_by_capture, bpsamp/8 most probably.
    short bpsamp; // bits_per_sample: CD quality is 16.
    char datastr[4]; // should always contain "data"
    int bid; // bytes_in_data;
} wh_t; /* wav header type */

long fszfind(FILE *fp)
{
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    long fbytsz = ftell(fp);
    rewind(fp);
    return fbytsz;
}

wh_t *initbarewav1(void) /* uses strncpy */
{
    wh=calloc(sizeof(wh_t));
    strncpy(wh->id, "RIFF", 4);
    strncpy(wh->fstr, "WAVEfmt ", 8);
    strncpy(wh->datastr, "data", 4);
    return wh;
}


wh_t *initnormwav(int sfre, char nucha) /* init a normal wav .... i.e. with 1 and 16 in it */
{
    wh=calloc(sizeof(wh_t));
    strncpy(wh->id, "RIFF", 4);
    strncpy(wh->fstr, "WAVEfmt ", 8);
    fmtnum=16;
    pcnum=1;
    nchans=nuhca; /* fed in to function */
    strncpy(wh->datastr, "data", 4);
    return wh;
    int glen;    // general length: total file length minus 8, could because the types so far seen (id and glen itself) are actually 8 bytes
    char fstr[8]; // format string should be "WAVEfmt ": actually two 4chartypes here.
    int fmtnum; // format number 16 for PCM format
    short pcmnum; // PCM number 1 for PCM format
    short nchans; // num channels
    int sampfq; // sampling frequency: CD quality is 44100, 48000 is also common.
    int bps; // bytes_per_second (aka, BlockAlign) = numchannels* bytes per sample*samplerate. stereo shorts at 44100 . should be 176k.
    short bbc; // bytes_by_capture, bpsamp/8 most probably.
    short bpsamp; // bits_per_sample: CD quality is 16.
    char datastr[4]; // should always contain "data"
    int bid; // bytes_in_data;
} wh_t; /* wav header type */
void wavdiscover(char *iwavname) /* we discover how wellformed the wavfile header is, and whther it needs coaxing */
{
    FILE *wavfp;
    wavfp = fopen(iwavname,"rb");
    if ( wavfp == NULL ) {
        fprintf(stderr,"Can't open input file %s", iwavname);
        exit(EXIT_FAILURE);
    }

    long iwsz=fszfind(wavfp);

    // read header
    wh_t inhdr;
    size_t wh_tsz= sizeof(wh_t);
    printf("wh_t size is %zu.\n", wh_tsz);


    if ( fread(&inhdr, wh_tsz, 1, wavfp) < 1 ) { /* notice how incredibly great this call is */
        printf("Can't read file header\n");
        exit(EXIT_FAILURE);
    }

    if

    /* OK .. we test what sort of header we have */
    if(memcmp(inhdr.id, 0x52494646) != 0) { /* this is "RIFF" in 4 char hex */
        printf("ERROR: RIFF string problem\n"); 
        exit(EXIT_FAILURE);
    }

    if ( inhdr.fstr[0] != 'W'
            || inhdr.fstr[1] != 'A' 
            || inhdr.fstr[2] != 'V' 
            || inhdr.fstr[3] != 'E' 
            || inhdr.fstr[4] != 'f'
            || inhdr.fstr[5] != 'm' 
            || inhdr.fstr[6] != 't'
            || inhdr.fstr[7] != ' ' ) { 
        printf("ERROR: WAVEfmt string problem\n"); 
        exit(EXIT_FAILURE);
    }

    if ( inhdr.datastr[0] != 'd'
            || inhdr.datastr[1] != 'a' 
            || inhdr.datastr[2] != 't' 
            || inhdr.datastr[3] != 'a' ) { 
        printf("WARNING: header \"data\" string does not come up\n"); 
    }
    if ( inhdr.fmtnum != 16 )
        printf("WARNING: fmtnum is %i, while it's better off being %i\n", inhdr.fmtnum, 16); 
    
    printf("Generally an O.K. wav format. Other dstruct values:\n");

    printf("glen: %i\n", inhdr.glen);
    printf("bid: %i\n", inhdr.bid);
    printf("fmtnum: %i\n", inhdr.fmtnum);
    printf("pcmnum: %d\n", inhdr.pcmnum);
    printf("nchans: %d\n", inhdr.nchans);
    printf("sampfq: %i\n", inhdr.sampfq);
    printf("bps: %i\n", inhdr.bps);
    printf("bbc, bytes by capture (count of data in shorts): %d\n", inhdr.bbc);
    printf("bpsamp: %d\n", inhdr.bpsamp);

    if(inhdr.glen+8-wh_tsz == inhdr.bid)
        printf("Good, \"bid\" (%i) is 36 bytes smaller than \"glen\" (%i).\n", inhdr.bid, inhdr.glen);
    else {
        printf("WARNING: glen (%i) and bid (%i)do not show prerequisite normal relation(diff is %i).\n", inhdr.glen, inhdr.bid, inhdr.glen-inhdr.bid); 
    }
    // printf("Duration by glen is: %f\n", (float)(inhdr.glen+8-wh_tsz)/(inhdr.nchans*inhdr.sampfq*inhdr.bps));
    printf("Duration by bps is: %f\n", (float)(inhdr.glen+8-wh_tsz)/inhdr.bps);

    if( (inhdr.bbc == inhdr.bps/inhdr.sampfq) && (inhdr.bbc == 2) )
        printf("bbc complies with being 2 and matching bps/sampfq. Data values can therefore be recorded as signed shorts.\n"); 

    /*now let's use some basic fseek type functions to see if all adds up */
    rewind(wavfp);
    if(inhdr.glen == lenf-8) 
    printf("ftell and glen in agreement with one another:  %lu\n", lenf-44); 
    
    fseek(wavfp, 44L, SEEK_SET);

    if(md == INFONLY) /* the info part is finished. */
        return 0;

    /* OK the file has been read in and checked, now let's calculate the size of the output file */
    size_t sapermin=inhdr.sampfq*60, sapervig=inhdr.sampfq/20;
    size_t edista= (stamin*sapermin + stasec*inhdr.sampfq + stavig*sapervig) *inhdr.nchans *sizeof(short);
    size_t edisz= (endmin*sapermin + endsec*inhdr.sampfq + endvig*sapervig) *inhdr.nchans *sizeof(short) - edista;

    wh_t ohd;
    memcpy(&ohd, &inhdr, sizeof(wh_t)); /* let's just blanket copy the inhdr to ohdr and then chaneg the values that need changing. */
    ohd.bid = edisz;
    ohd.glen=ohd.bid+36;

    short *outbuf=malloc(sizeof(wh_t)+edisz);
    memcpy(outbuf, &ohd, sizeof(wh_t));

    if(ftell(wavfp) != 44) {
        printf("Somethings up with the position in the read file, it's not 44.\n");  
        exit(EXIT_FAILURE);
    }
    fseek(wavfp, edista, SEEK_CUR);

    size_t nread =fread(outbuf+44, sizeof(char), edisz, wavfp);
    // if( (nread =fread(outbuf+44, sizeof(char), edisz, wavfp)) != edisz) { /* note how edisz already incorporates short size */
    if( nread <=0) {
        printf("Prob with fread. Aborting.\n"); 
        free(outbuf);
        exit(EXIT_FAILURE);
    }
    fclose(wavfp);

    FILE *owavfp = fopen(owavname,"wb");
    fwrite(outbuf, sizeof(char), edisz+44, owavfp);

    fclose(owavfp);
    free(outbuf);
    return 0;
}
int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: one filename argument needed: if name ends with \"wav\" extension, analyses a wav file.\n");
        printf("Otherwise argument should be a text file with wav editing data.\n");
        exit(EXIT_FAILURE);
    }

    char *xt = NULL;
    xt=strrchr(argv[1], '.');
    enum modenum md;

    char iwavname[ARBSZ];
    char owavname[ARBSZ];
    FILE *fpa, *wavfp;

        strcpy(iwavname, argv[1]);

    wavfp = fopen(iwavname,"rb");
    if ( wavfp == NULL ) {
        fprintf(stderr,"Can't open input file %s", iwavname);
        exit(EXIT_FAILURE);
    }

    // read header
    wh_t inhdr;
    size_t wh_tsz= sizeof(wh_t);
    printf("wh_t size is %zu.\n", wh_tsz);


    if ( fread(&inhdr, wh_tsz, 1, wavfp) < 1 ) { /* notice how incredibly great this call is */
        printf("Can't read file header\n");
        exit(EXIT_FAILURE);
    }

    /* OK .. we test what sort of header we have */
    if ( inhdr.id[0] != 'R'
            || inhdr.id[1] != 'I' 
            || inhdr.id[2] != 'F' 
            || inhdr.id[3] != 'F' ) { 
        printf("ERROR: RIFF string problem\n"); 
        exit(EXIT_FAILURE);
    }

    if ( inhdr.fstr[0] != 'W'
            || inhdr.fstr[1] != 'A' 
            || inhdr.fstr[2] != 'V' 
            || inhdr.fstr[3] != 'E' 
            || inhdr.fstr[4] != 'f'
            || inhdr.fstr[5] != 'm' 
            || inhdr.fstr[6] != 't'
            || inhdr.fstr[7] != ' ' ) { 
        printf("ERROR: WAVEfmt string problem\n"); 
        exit(EXIT_FAILURE);
    }

    if ( inhdr.datastr[0] != 'd'
            || inhdr.datastr[1] != 'a' 
            || inhdr.datastr[2] != 't' 
            || inhdr.datastr[3] != 'a' ) { 
        printf("WARNING: header \"data\" string does not come up\n"); 
    }
    if ( inhdr.fmtnum != 16 )
        printf("WARNING: fmtnum is %i, while it's better off being %i\n", inhdr.fmtnum, 16); 
    
    printf("Generally an O.K. wav format. Other dstruct values:\n");

    printf("glen: %i\n", inhdr.glen);
    printf("bid: %i\n", inhdr.bid);
    printf("fmtnum: %i\n", inhdr.fmtnum);
    printf("pcmnum: %d\n", inhdr.pcmnum);
    printf("nchans: %d\n", inhdr.nchans);
    printf("sampfq: %i\n", inhdr.sampfq);
    printf("bps: %i\n", inhdr.bps);
    printf("bbc, bytes by capture (count of data in shorts): %d\n", inhdr.bbc);
    printf("bpsamp: %d\n", inhdr.bpsamp);

    if(inhdr.glen+8-wh_tsz == inhdr.bid)
        printf("Good, \"bid\" (%i) is 36 bytes smaller than \"glen\" (%i).\n", inhdr.bid, inhdr.glen);
    else {
        printf("WARNING: glen (%i) and bid (%i)do not show prerequisite normal relation(diff is %i).\n", inhdr.glen, inhdr.bid, inhdr.glen-inhdr.bid); 
    }
    // printf("Duration by glen is: %f\n", (float)(inhdr.glen+8-wh_tsz)/(inhdr.nchans*inhdr.sampfq*inhdr.bps));
    printf("Duration by bps is: %f\n", (float)(inhdr.glen+8-wh_tsz)/inhdr.bps);

    if( (inhdr.bbc == inhdr.bps/inhdr.sampfq) && (inhdr.bbc == 2) )
        printf("bbc complies with being 2 and matching bps/sampfq. Data values can therefore be recorded as signed shorts.\n"); 

    /*now let's use some basic fseek type functions to see if all adds up */
    rewind(wavfp);
    if(inhdr.glen == lenf-8) 
    printf("ftell and glen in agreement with one another:  %lu\n", lenf-44); 
    
    fseek(wavfp, 44L, SEEK_SET);

    if(md == INFONLY) /* the info part is finished. */
        return 0;

    /* OK the file has been read in and checked, now let's calculate the size of the output file */
    size_t sapermin=inhdr.sampfq*60, sapervig=inhdr.sampfq/20;
    size_t edista= (stamin*sapermin + stasec*inhdr.sampfq + stavig*sapervig) *inhdr.nchans *sizeof(short);
    size_t edisz= (endmin*sapermin + endsec*inhdr.sampfq + endvig*sapervig) *inhdr.nchans *sizeof(short) - edista;

    wh_t ohd;
    memcpy(&ohd, &inhdr, sizeof(wh_t)); /* let's just blanket copy the inhdr to ohdr and then chaneg the values that need changing. */
    ohd.bid = edisz;
    ohd.glen=ohd.bid+36;

    short *outbuf=malloc(sizeof(wh_t)+edisz);
    memcpy(outbuf, &ohd, sizeof(wh_t));

    if(ftell(wavfp) != 44) {
        printf("Somethings up with the position in the read file, it's not 44.\n");  
        exit(EXIT_FAILURE);
    }
    fseek(wavfp, edista, SEEK_CUR);

    size_t nread =fread(outbuf+44, sizeof(char), edisz, wavfp);
    // if( (nread =fread(outbuf+44, sizeof(char), edisz, wavfp)) != edisz) { /* note how edisz already incorporates short size */
    if( nread <=0) {
        printf("Prob with fread. Aborting.\n"); 
        free(outbuf);
        exit(EXIT_FAILURE);
    }
    fclose(wavfp);

    FILE *owavfp = fopen(owavname,"wb");
    fwrite(outbuf, sizeof(char), edisz+44, owavfp);

    fclose(owavfp);
    free(outbuf);
    return 0;
}
