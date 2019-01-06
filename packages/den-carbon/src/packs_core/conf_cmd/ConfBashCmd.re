open Bootstrap;

include Command.AsyncWithResult.Make(
          (
            FileEditorCmd.Make({
              open Args.Arg;
              let command = "bash";
              let description = "Edit your bash configuration file";
              let pathDescription = "The location of your bash profile";
              let pathRequired =
                Default(DenSeed.Os.(Platform.value |> Bashrc.location));
            })
          ),
        );
