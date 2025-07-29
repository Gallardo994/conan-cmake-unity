# Sample Library

This is a project that demonstrates how to create and compile a simple C++ library using CMake with Conan for dependency management, suitable for production use in Unity.

Tested on: Unity 2022.3.62f1

Compatible platforms:
- MacOS (arm64)
- Android (arm64, x86_64)
- iOS (iphoneos, simulator)

Pretty much any platform supported by Unity should work, but the project is roughly tested on the above platforms.

Windows isn't much different, but it has not been tested with this specific project setup. I recommend using MinGW + LLVM Clang for Windows builds, as it is the most compatible with the project setup, meaning you would be able to have the same build pipelines for all platforms.

Included packages for testing purposes:
- `glm/1.0.1` - used for quick calculation of a three-dimensional vector length.
- `libcurl/8.12.1` - used to demonstrate how to use a third-party library in the project, and to show basic pointer/string manipulation coming from C++ to C#.

Tested compilers:
- Apple Clang 17.0.0 (MacOS and iOS builds).
- Clang 19.0.1 (Android builds) bundled with Android NDK 28c.

C++ version is set to C++20, but you can change it in the root `CMakeLists.txt` file if needed. Don't forget to change the standard in the profiles as well.

## Prerequisites
- CMake 3.22 or higher (latest version recommended)
- Conan2 (latest version recommended), which itself depends on Python 3.
- Unity 2022.3.62f1, but any should work. 6.X are as compatible as 2021 and lower.
- **(Android)** Android NDK (for Android builds) set in `profiles/android-*` profiles.
- **(iOS/Simulator)** Xcode (for iOS and macOS builds) installed with iphoneos and simulator SDKs defined in `profiles/ios-*` profiles.

## Compiling the library

The example will use macos-arm64 as the target triplet, but you can use any other preset triplet for your platform (`profiles/` directory).
You may also create your own profile if needed.

1. Open a terminal and navigate to the root directory of the project.
2. Run the following command to install dependencies:
    ```bash
    conan install . --profile=profiles/macos-arm64-release --build=missing
    ```
3. After the dependencies are installed, run the following command to build the library:
    ```bash
    conan build . --profile=profiles/macos-arm64-release
    ```
4. The compiled library will be located in the `build` directory, typically inside `build/modules/samplelibrary/` directory. In MacOS case, the artifact will be the `libsamplelibrary.dylib` file.

## Using the library in Unity

The libraries have to be placed in the `Plugins` directory of any subfolder of the Unity project. A good rule of thumb is to follow this structure:
```
Assets/
  Plugins/
    MacOS/
      arm64/
        libsamplelibrary.dylib
      x86_64/
        libsamplelibrary.dylib
    Android/
      arm64-v8a/
        libsamplelibrary.so
      x86_64/
        libsamplelibrary.so
    iOS/
      libsamplelibrary.a
      libsamplelibrary_simulator.a
    Windows/
      x86_64/
        samplelibrary.dll
```

Make sure to apply appropriate import settings for each platform in Unity. For example, for iOS, you should set the library to be used for both iOS and Simulator platforms. MacOS and Windows libraries might also need to have their import settings allow their usage in the Editor.

Example bindings for the current project are as follows:
```csharp
using System;
using System.Runtime.InteropServices;

public static class Bindings
{
#if UNITY_IOS && !UNITY_EDITOR
    private const string Library = "__Internal";
#else
    private const string Library = "libsamplelibrary";
#endif

    [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
    public static extern int sample_library_add(int a, int b);

    [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
    public static extern int sample_library_multiply(int a, int b);

    [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
    public static extern float sample_library_vector_length(float x, float y, float z);

    [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr sample_library_curl_current_version();
}
```

You can create a C# script in Unity and use the bindings like this:
```csharp
using UnityEngine;
using System.Runtime.InteropServices;

public class Startup : MonoBehaviour
{
    private void Update()
    {
        Debug.Log($"1 + 2 = {Bindings.sample_library_add(1, 2)}");
        Debug.Log($"3 * 4 = {Bindings.sample_library_multiply(3, 4)}");
        Debug.Log("Vector length of (1, 2, 3) = " + Bindings.sample_library_vector_length(1f, 2f, 3f));

        var versionPtr = Bindings.sample_library_curl_current_version();
        Debug.Log("Current version of libcurl: " + Marshal.PtrToStringAnsi(versionPtr));

        // Expected output:
        // 1 + 2 = 3
        // 3 * 4 = 12
        // Vector length of (1, 2, 3) = 3.741657
        // Current version of libcurl: libcurl/8.12.1 SecureTransport zlib/1.3.1
    }
}
```

## Gotchas

- **Updating the libraries**: You may need to restart Unity after updating the libraries in the `Plugins` directory to ensure that Unity recognizes the changes. You can theoretically manually load the libraries bypassing `DllImport` whatsoever, and this will allow you to update the libraries without restarting Unity, but this is not covered in this sample project.
- **iOS Static Linking**: iOS requires static linking for libraries when used inside Unity. Existing presets configure that for you, but remember that symbols may clash if you use multiple libraries with the same symbols. If you encounter linker errors, or, worse, runtime crashes that happen exclusively on iOS platforms, check for symbol conflicts. They may also conflict with Unity's own libraries, so be careful with the names of your functions.
- **Android 16KB Alignment Requirement**: Starting Nov. 2025, Google Play requires all native libraries to be aligned to 16KB. This is already configured in the `CMakeLists.txt` file, but only if your NDK version supports `APP_SUPPORT_FLEXIBLE_PAGE_SIZES`. Refer to the [Official Documentation](https://developer.android.com/guide/practices/page-sizes) for more details.
- **Quick iterations on Android**: If you're doing quick iterations on Android, you can repackage the .apk file by patching the `libsamplelibrary.so` file inside the .apk file. This can be done using, well, a zip archiver and/or `apktool`. This is not covered in this sample project, but it can save you a lot of time during development.
