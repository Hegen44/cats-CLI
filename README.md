# cats-CLI

A simple CLI that handle a resource cats withi commands for create, read, ujpdate, and delete.

## Getting Started

### Executing program

* Run Cats.exe with any command prompt

## Help

### Usage
```
cats <command> [<args>]
```
### Available commands
#### Create
Create and add cats data to the file.
```
cats create [<args>]
```
* Options:
   * `--name <name>` - add name to the new cat.
   * `--breed <bread>` - add breed to the new cat.
   * `--age <age>` - add age to the new cat. Only accept positive number.
#### Read
List all cats entry in the file.
```
cats read
```
#### Update
Edit existing in the file by their corresponding id.
```
cats update [<args>]
```
* Options:
  * `<id>` - indexes of cats to update content from.
  * `--name <name>` - update only the name of the cats.
  * `--breed <breed` - update only the breed of the cats.
  * `--age <age>`" - update only the age of the cats. Only accept positive number.
#### Delete
Delete cats from file.
```
cats delete [<args>]
```
* Options:
  * `<id>` - indexes of cats to remove.
  * `--all` - Delete all the existing records in the file.
#### Help
Display help information about Cats.
```
cats help
```
