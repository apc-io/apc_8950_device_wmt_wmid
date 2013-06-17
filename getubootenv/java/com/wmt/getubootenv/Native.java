/*++
Copyright (c) 2011-2012 WonderMedia Technologies, Inc.

Licensed under the Apache License, Version 2.0 (the "License"); 
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
OF ANY KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
--*/
package com.wmt.getubootenv;

import java.io.File;
//import android.util.Log;

public final class Native {
	static final String TAG = "UbootEnv";
	static final String NATIVE_SO = "/system/lib/libgetubootenv.so";
	public static boolean isValid = true;
	
    static {
    	//for debug on emulator
    	File sof = new File(NATIVE_SO);
    	if (!sof.exists()) {
            isValid = false;
        }
    	
    	// The runtime will add "lib" on the front and ".so" on the end of
    	// the name supplied to loadLibrary.
    	if (isValid) System.loadLibrary("getubootenv");
    }    
       
    public static native int setEnv(String name, String value);
    public static native String getEnv(String name);
}
