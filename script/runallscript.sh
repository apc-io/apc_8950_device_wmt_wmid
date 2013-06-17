################################################################################
#                                                                              #
# Copyright c 2009-2011  WonderMedia Technologies, Inc.   All Rights Reserved. #
#                                                                              #
# This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.  #
# and may contain trade secrets and/or other confidential information of       #
# WonderMedia Technologies, Inc. This file shall not be disclosed to any third #
# party, in whole or in part, without prior written consent of WonderMedia.    #
#                                                                              #
# THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,  #
# WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED, #
# AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED          #
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET       #
# ENJOYMENT OR NON-INFRINGEMENT.                                               #
#                                                                              #
################################################################################

for i in /etc/rc.d/rc0.d/S??*
do
        # Ignore dangling symlinks for now.
        [ ! -f "$i" ] && continue
	#echo $i
        case "$i" in
                *.sh)
                        # Source shell script for speed.
                        (
                                trap - INT QUIT TSTP
                                set start
                                . $i 2>/dev/null
                                #. $i 1>/dev/null 2>/dev/null
                        )
                        ;;
                *)
                        # No sh extension, so fork subprocess.
                        $i start 2>/dev/null
			#$i start 1>/dev/null 2>/dev/null
                        ;;
        esac
done
