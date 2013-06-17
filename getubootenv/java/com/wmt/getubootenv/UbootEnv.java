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

//import android.util.Log;

public final class UbootEnv {
	static final String TAG = "UbootEnv";
	
    public static int setEnv(String name, String value) {
    	if (Native.isValid) {
    	    return Native.setEnv(name, value);
    	} else {
	        return -999;
	}
    }
    
    public static String getEnv(String name) {
    	if (Native.isValid) {
    		return Native.getEnv(name);
    	} else {
    		return "";
    	}
    }
}
