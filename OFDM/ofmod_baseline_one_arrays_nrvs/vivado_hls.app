<AutoPilot:project xmlns:AutoPilot="com.autoesl.autopilot.project" projectType="C/C++" name="ofmod_baseline_one_arrays_nrvs" top="ofmodulating">
    <Simulation argv="0">
        <SimFlow name="csim" setup="false" optimizeCompile="false" clean="false" ldflags="" mflags=""/>
    </Simulation>
    <includePaths/>
    <libraryFlag/>
    <files>
        <file name="../../../lte_phy.cpp" sc="0" tb="1" cflags=" -I../../.. "/>
        <file name="../../../lib/gauss.cpp" sc="0" tb="1" cflags=" -I../../../lib "/>
        <file name="../../../lib/GeneralFunc.cpp" sc="0" tb="1" cflags=" -I../../../lib "/>
        <file name="../../OFDMMain.cpp" sc="0" tb="1" cflags=" -I../../../.  -I../../../lib "/>
        <file name="../fft/fft_base.cpp" sc="0" tb="false" cflags="-I../. -I../lib"/>
        <file name="OFDM_baseline.cpp" sc="0" tb="false" cflags="-I../. -I../lib"/>
    </files>
    <solutions>
        <solution name="solution1" status=""/>
    </solutions>
</AutoPilot:project>

