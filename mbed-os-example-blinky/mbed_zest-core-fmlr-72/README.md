# Zest_Core_FMLR-72
[Zest_Core_FMLR-72](https://gitlab.com/catie_6tron/zest-core-fmlr-72-hardware)
custom target for Mbed OS.

## Usage
In your project root directory:

1.  Add the custom target to your project:

    ```shell
    mbed add https://gitlab.com/catie_6tron/zest-core-fmlr-72.git
    ```

2. Enable the custom target by adding or overwriting the `custom_targets.json` at the
   root of the project:

    ```shell
    cp zest-core-fmlr-72/custom_targets.json .
    ```

3. Compile for the custom target:

   ```shell
   mbed compile --target ZEST_CORE_FMLR-72
   ```
