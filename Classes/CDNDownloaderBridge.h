#ifndef __CDN_DOWNLOADER_BRIDGE_H__
#define __CDN_DOWNLOADER_BRIDGE_H__

typedef void (*CDNProgressCallback)(float percent, long long downloaded, long long total);
typedef void (*CDNSuccessCallback)();
typedef void (*CDNErrorCallback)(const char* error);

#ifdef __cplusplus
extern "C" {
#endif

void startNativeDownload(const char* url, const char* destinationPath,
                         CDNProgressCallback onProgress,
                         CDNSuccessCallback onSuccess,
                         CDNErrorCallback onError);

#ifdef __cplusplus
}
#endif

#endif // __CDN_DOWNLOADER_BRIDGE_H__
