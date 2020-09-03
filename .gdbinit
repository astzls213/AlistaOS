set startup-with-shell off
set $lastcs = -1

define hook-stop
  if $cs == 8 || $cs == 27
    if $lastcs != 8 && $lastcs != 27
      set architecture i386
    end
    x/i $pc
  else
    if $lastcs == -1 || $lastcs == 8 || $lastcs == 27
      set architecture i8086
    end
    # Real mode
    printf "[%4x:%4x] ", $cs, $eip
    x/i $cs*16+$eip
  end
  set $lastcs = $cs
end

echo + target remote localhost:1234\n
target remote localhost:1234


echo + symbol-file obj/boot/boot.elf\n
symbol-file obj/boot/boot.elf

