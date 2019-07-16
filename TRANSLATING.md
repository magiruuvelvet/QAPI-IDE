# Translating QApi IDE

## Requirements

 - Qt Linguist Tools (`lupdate`, `lrelease`)
 - Qt Linguist to work with the generated `.ts` files

### Generating a new translation

Inside the source code repository run:

```sh
./tools/create-translation en_US
```

where `en_US` is replaced with the language ISO code.

This will create a new file under `UserInterface/i18n/en_US.ts`
with all strings found during analyzing the source tree.

### Updating existing translations

Inside the source code repository run:

```sh
./tools/update-translations
```

This will rescan the source tree for newly added and deleted strings
and update all `.ts` files accordingly.

