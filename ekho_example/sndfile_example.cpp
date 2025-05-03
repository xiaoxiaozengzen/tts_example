#include <sndfile.h>

#include <iostream>
#include <stdlib.h>

int sndfile_example() {
  const char *filename = "/mnt/workspace/cgz_workspace/Exercise/tts_example/data/nihao.wav";

  /**
   * A SNDFILE* pointer can be passed around much like stdio.h's FILE* pointer
   */
  SNDFILE *sndfile = nullptr;

  /**
   * struct SF_INFO {
   *   sf_count_t frames;      // 采样帧数
   *   int samplerate;        // 采样率
   *   int channels;          // 通道数
   *   int format;           // 格式
   *   int sections;         // 段数
   *   int seekable;         // 是否可寻址
   * };
   */
  SF_INFO sfinfo = {0};
  sfinfo.format = 0; // 当读取文本时，需要预设为0

  /**
   * @brief 读取wav文件
   * @param path wav文件路径
   * @param mode 读取模式
   * @param sfinfo wav文件信息
   * @return SNDFILE* 读取结果
   * 
   * mode: 
   * SFM_READ 只读
   * SFM_WRITE 只写
   * SFM_RDWR 读写
   */
  sndfile = sf_open(filename, SFM_READ, &sfinfo);
  if(sndfile == nullptr) {
    printf("Error opening file: %s\n", sf_strerror(sndfile));
    return -1;
  }
  std::cout << "sinfo: " << std::endl;
  std::cout << "frames: " << sfinfo.frames << std::endl;
  std::cout << "samplerate: " << sfinfo.samplerate << std::endl;
  std::cout << "channels: " << sfinfo.channels << std::endl;
  std::cout << "format: " << sfinfo.format << std::endl;
  std::cout << "sections: " << sfinfo.sections << std::endl;
  std::cout << "seekable: " << sfinfo.seekable << std::endl;

  short *buffer = (short *)malloc(sfinfo.frames * sfinfo.channels * sizeof(short));
  if(buffer == nullptr) {
    std::cerr << "Error allocating memory for buffer" << std::endl;
    sf_close(sndfile);
    return -1;
  }

  /**
   * @brief 读取音频数据
   * 
   * @param sndfile SNDFILE* 指针
   * @param ptr 指向数据的指针
   * @param frames 读取的帧数
   * 
   * @return sf_count_t 实际读取的帧数
   */
  sf_count_t frames_read = sf_readf_short(sndfile, buffer, sfinfo.frames);
  if(frames_read < 0) {
    std::cerr << "Error reading frames: " << sf_strerror(sndfile) << std::endl;
    free(buffer);
    sf_close(sndfile);
    return -1;
  }
  std::cout << "Read " << frames_read << " frames" << std::endl;
  std::cout << "First sample: " << buffer[0] << std::endl;

  free(buffer);
  sf_close(sndfile); 
  
  return 0;
}

/**
 * WAV文件的基本结构：
 * 1. RIFF块：包含文件类型、文件大小等信息
 * 2. fmt块：包含音频格式、采样率、通道数等信息
 * 3. data块：包含音频数据
 */
void wave_example() {
  const char *filename = "/mnt/workspace/cgz_workspace/Exercise/tts_example/data/nihao.wav";

  struct WAVHeader {
    char riff[4];          // "RIFF"
    uint32_t size;        // 文件大小
    char wave[4];         // "WAVE"
    char fmt[4];          // "fmt "
    uint32_t fmt_size;    // fmt块大小
    uint16_t format_tag;  // 格式类型
    uint16_t channels_num; // 通道数
    uint32_t sample_rate;  // 采样率
    uint32_t avg_bytes_per_sec; // 平均字节每秒
    uint16_t block_align;  // 块对齐
    uint16_t bits_per_sample; // 采样位数
    char data[4];         // "data"
    uint32_t data_size;   // 数据大小
  };

  FILE *file = fopen(filename, "rb");
  if(file == nullptr) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  WAVHeader header;
  /**
   * @brief 读取wav文件头
   * @param ptr 指向wav文件头的指针
   * @param size 数据单元大小，一般是sizeof(WAVHeader)
   * @param count 读取的单元个数。总读取字节为 size * count
   * @param stream 文件指针，使用fopen打开的文件指针
   * 
   * @return size_t 实际读取的单元个数
   * 
   * @note 每次调用fread函数，文件指针会向后移动size * count个字节
   * @note 如果连续调用fread函数，每次读取数据会从上一次读取结束的位置开始
   */
  size_t read_size = fread(&header, sizeof(WAVHeader), 1, file);
  if(read_size != 1) {
    std::cerr << "Error reading file header" << std::endl;
    fclose(file);
    return;
  }

  std::cout << "RIFF: " << std::string(header.riff, 4) << std::endl;
  std::cout << "Size: " << header.size << std::endl;
  std::cout << "WAVE: " << std::string(header.wave, 4) << std::endl;
  std::cout << "FMT: " << std::string(header.fmt, 4) << std::endl;
  std::cout << "Format Size: " << header.fmt_size << std::endl;
  std::cout << "Format Tag: " << header.format_tag << std::endl;
  std::cout << "Channels: " << header.channels_num << std::endl;
  std::cout << "Sample Rate: " << header.sample_rate << std::endl;
  std::cout << "Avg Bytes Per Sec: " << header.avg_bytes_per_sec << std::endl;
  std::cout << "Block Align: " << header.block_align << std::endl;
  std::cout << "Bits Per Sample: " << header.bits_per_sample << std::endl;
  std::cout << "Data: " << std::string(header.data, 4) << std::endl;
  std::cout << "Data Size: " << header.data_size << std::endl;

  int num_samples = header.data_size / (header.bits_per_sample / 8);
  std::cout << "Number of Samples: " << num_samples << std::endl;

  short *buffer = (short *)malloc(num_samples * sizeof(short));
  if(buffer == nullptr) {
    std::cerr << "Error allocating memory for buffer" << std::endl;
    fclose(file);
    return;
  }
  int read_samples = fread(buffer, sizeof(short), num_samples, file);
  if(read_samples != num_samples) {
    std::cerr << "Error reading audio data" << std::endl;
    free(buffer);
    fclose(file);
    return;
  }
  std::cout << "Read " << read_samples << " samples" << std::endl;
  std::cout << "First sample: " << buffer[0] << std::endl;

  fclose(file);
}

int main() {
  std::cout << "=======================sndfile_example========================" << std::endl;
  sndfile_example();
  std::cout << "=======================wave_example========================" << std::endl;
  wave_example();
  return 0;
}