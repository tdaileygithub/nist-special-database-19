#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
#include "imgui/implot.h"
#include <random>

struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                  FenceValue;
};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void WaitForLastSubmittedFrame();

FrameContext* WaitForNextFrameResources();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Imgui_Render();
void Imgui_Cleanup();
void Imgui_SetupContext();

// Data
extern struct FrameContext                  g_frameContext[];
extern UINT                                 g_frameIndex;
extern ID3D12Device*                        g_pd3dDevice;
extern ID3D12DescriptorHeap*                g_pd3dRtvDescHeap;
extern ID3D12DescriptorHeap*                g_pd3dSrvDescHeap;
extern ID3D12CommandQueue*                  g_pd3dCommandQueue;
extern ID3D12GraphicsCommandList*           g_pd3dCommandList;
extern ID3D12Fence*                         g_fence;
extern HANDLE                               g_fenceEvent;
extern UINT64                               g_fenceLastSignaledValue;
extern IDXGISwapChain3*                     g_pSwapChain;
extern HANDLE                               g_hSwapChainWaitableObject;
extern struct ID3D12Resource*               g_mainRenderTargetResource[];
extern struct D3D12_CPU_DESCRIPTOR_HANDLE   g_mainRenderTargetDescriptor[];

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};
// utility structure for realtime plot
struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};