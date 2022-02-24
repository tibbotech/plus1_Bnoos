#! /bin/bash

TOPDIR=$1
OUT_PATH=${TOPDIR}/bin
mkdir -p ${OUT_PATH}
#####################################generate the arm926 dev list
ARM926_LIST=${OUT_PATH}/arm926_list

VARIANT_FILE=${TOPDIR}/variants/sp7021_evb/variant.h
VARIANT_TEMP=${OUT_PATH}/variant.h_temp

#generate the temporary copy of variant.h
cp ${VARIANT_FILE} ${VARIANT_TEMP}
#drop the line include '//'
sed -i 's/^\s*//g' ${VARIANT_TEMP}
sed -i '/^\/\//d' ${VARIANT_TEMP}
#drop the block include '#if 0 #endif'
sed -i '/#if 0/,/#endif/d' ${VARIANT_TEMP}

sed -n -e '/SERIAL_UART_INSTANCE/{s/^.*0.*$/uart0/p'\
        -e 's/^.*1.*$/uart1/p'\
        -e 's/^.*2.*$/uart2/p'\
        -e 's/^.*3.*$/uart3/p'\
        -e 's/^.*4.*$/uart4/p}'\
        -e '/SPI_SEL_INSTANCE/{s/^.*0.*$/spi_controller0/p'\
        -e 's/^.*1.*$/spi_controller1/p'\
        -e 's/^.*2.*$/spi_controller2/p'\
        -e 's/^.*3.*$/spi_controller3/p}'\
        -e '/I2C_SEL_INSTANCE/{s/^.*0.*$/i2cm0/p'\
        -e 's/^.*1.*$/i2cm1/p'\
        -e 's/^.*I2C.*2.*$/i2cm2/p'\
        -e 's/^.*3.*$/i2cm3/p}'\
        -e '/ICM_SEL_INSTANCE/{s/^.*$/icm/p}'\
        ${VARIANT_TEMP} > ${ARM926_LIST}

#debug print temp
#cat ${ARM926_LIST}

####################################generate the ca7 dev list
CA7_LIST=${TOPDIR}/bin/ca7_list

CONFIG_ROOT=${TOPDIR}/../../.config
DTS_CONFIG_ROOT=${TOPDIR}/../../linux/kernel/arch/arm/boot/dts
LINUX_DTS=$(cat ${CONFIG_ROOT} | grep 'LINUX_DTB=' | sed 's/LINUX_DTB=//g').dts
DTS_TEMP=${OUT_PATH}/${LINUX_DTS}_temp

#generate the temporary copy of dts file
cp ${DTS_CONFIG_ROOT}/${LINUX_DTS} ${DTS_TEMP}
#drop the line include '//'
sed -i 's/^\s*//g' ${DTS_TEMP}
sed -i '/^\/\//d' ${DTS_TEMP}
#drop the block include '#if 0 #endif'
sed -i '/#if 0/,/#endif/d' ${DTS_TEMP}

#sed -n '/^&/s/{/Fhead/;/^&/{:a;N;s/\n};/\nFtail/;s/\n//g;/Ftail/p;/Ftail/!ba}' ${DTS_CONFIG_ROOT}/${LINUX_DTS} > ${CA7_LIST}
sed -n -e '/^&/s/{/Fhead/'\
	-e '/Fhead/{'\
	-e ':a'\
        -e 'N'\
        -e 's/\n};/\nFtail/g'\
        -e 's/\n//g'\
        -e '/Ftail/p'\
        -e '/Ftail/!ba}'\
        ${DTS_TEMP} > ${CA7_LIST}

sed -i '/disabled/d' ${CA7_LIST}
sed -i 's/^&\(.*\) Fhead.*$/\1/' ${CA7_LIST}

#debug print temp
#cat ${CA7_LIST}

###################################merge the list
DEV_LIST="${OUT_PATH}/dev_list"

cat ${CA7_LIST} > ${DEV_LIST}
sed -i '1 i CA7 DEV LIST:' ${DEV_LIST}
sed -i '$ a ARM926 DEV LIST:' ${DEV_LIST}
cat ${ARM926_LIST} >> ${DEV_LIST}

#debug print temp
#cat ${DEV_LIST}
rm ${ARM926_LIST} ${CA7_LIST}

###################################compare and output the conflict dev
TEMP="${OUT_PATH}/temp"
CONFLICT_DEV_LIST="${OUT_PATH}/conflict_dev_list"

sort ${DEV_LIST} | uniq -c > ${TEMP}

#cut space added by command uniq in header
sed -i 's/^\s*//g' ${TEMP}
sed -n 's/^2 \(.*\)$/\1/p' ${TEMP} > ${CONFLICT_DEV_LIST}

#debug print temp
#cat ${TEMP}
#cat ${CONFLICT_DEV_LIST}

if [ -s ${CONFLICT_DEV_LIST} ]; then
	echo "--------------------------------------------------------------"
	echo "[arm926 check] WARNING: Devices that conflicts with Linux:"
	cat ${CONFLICT_DEV_LIST}
	echo "Devices can't normally work.Please select other similar device"
	echo "--------------------------------------------------------------"
	rm ${TEMP} ${CONFLICT_DEV_LIST}
	exit 1
else
	echo "[arm926 check]info: no conflict dev. is OK!"
	rm ${TEMP} ${CONFLICT_DEV_LIST}
fi


