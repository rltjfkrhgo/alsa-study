// https://github.com/alsa-project/alsa-lib/blob/master/test/pcm_min.c

#include <iostream>
#include <alsa/asoundlib.h>

int main()
{
    int err = 0;

    // open pcm device
    snd_pcm_t* handle = nullptr;
    if((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        std::cerr << "snd_pcm_open() error: " << snd_strerror(err) << std::endl;
        return -1;
    }

    // set parameters
    if((err = snd_pcm_set_params(handle, 
                                 SND_PCM_FORMAT_S16_LE,  // sample format
                                 SND_PCM_ACCESS_RW_INTERLEAVED, 
                                 2,      // channel
                                 48000,  // sample rate
                                 1, 
                                 500000)) < 0)
    {
        std::cerr << "snd_pcm_set_params() error: " << snd_strerror(err) << std::endl;
        return -1;
    }

    // some random data
    unsigned char buffer[2 * 2 * 1024];
    for(int i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = random() & 0xff;
    }

    // make some noise
    snd_pcm_uframes_t wirteReqSize = sizeof(buffer) / 2 / 2;
    for(int i = 0; i < 16; i++)
    {
        snd_pcm_sframes_t frames = snd_pcm_writei(handle, 
                                                  buffer, 
                                                  wirteReqSize);  // frame 단위
        if(frames < 0)
        {
            frames = snd_pcm_recover(handle, frames, 0);
        }
        if(frames < 0)
        {
            std::cerr << "snd_pcm_writei failed: " << snd_strerror(frames) << std::endl;
            break;
        }
        if(0 < frames && frames < wirteReqSize)
        {
            std::cout << "short write (expected " << wirteReqSize << " wrote " << frames << ")" << std::endl;
        }
    }

    // drain the remaining samples
    // drop이면 남은 데이터 버림
    if((err = snd_pcm_drain(handle)) < 0)
    {
        std::cerr << "snd_pcm_drain failed: " << snd_strerror(err) << std::endl;
    }

    // close pcm device
    snd_pcm_close(handle);

    return 0;
}