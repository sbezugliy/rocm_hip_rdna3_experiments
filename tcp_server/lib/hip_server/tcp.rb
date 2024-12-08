# frozen_string_literal: true

module HipServer
  # TCP server class implementation
  class Tcp
    def initialize(*options)
      @server = TCPServer.new(*options)
    end

    def run(&block)
      loop do
        Thread.start(@server.accept) do |client|
          client.puts block.call(client.gets)
          client.close
        end
      end
    end
  end
end
